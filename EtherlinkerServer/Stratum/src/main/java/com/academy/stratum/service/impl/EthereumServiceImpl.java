package com.academy.stratum.service.impl;

import com.academy.contracts.MethodParamsTester;
import com.academy.stratum.dto.EtherlinkerBatchRequestData;
import com.academy.stratum.dto.EtherlinkerBatchResponseData;
import com.academy.stratum.dto.EtherlinkerRequestData;
import com.academy.stratum.dto.EtherlinkerResponseData;
import com.academy.stratum.service.EthereumService;
import com.academy.utils.AddressUtils;
import com.academy.utils.ApplicationContextProvider;
import com.academy.utils.RestException;
import org.apache.commons.io.FilenameUtils;
import org.bitcoinj.core.ECKey;
import org.bitcoinj.crypto.ChildNumber;
import org.bitcoinj.crypto.DeterministicKey;
import org.bitcoinj.crypto.HDKeyDerivation;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Service;
import org.web3j.crypto.*;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.admin.Admin;
import org.web3j.protocol.core.DefaultBlockParameterName;
import org.web3j.protocol.core.RemoteCall;
import org.web3j.protocol.core.methods.response.EthGetBalance;
import org.web3j.protocol.core.methods.response.TransactionReceipt;
import org.web3j.protocol.http.HttpService;
import org.web3j.tx.Contract;
import org.web3j.tx.Transfer;
import org.web3j.utils.Convert;

import java.io.File;
import java.lang.reflect.Method;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import static org.web3j.tx.gas.DefaultGasProvider.GAS_LIMIT;
import static org.web3j.tx.gas.DefaultGasProvider.GAS_PRICE;

/**
 * Implementation of {@link EthereumService} interface
 * Allows you to interact with Ethereum blockchain
 *
 * @author Ruslan Nazirov
 */
@Service("ethereumService")
public class EthereumServiceImpl implements EthereumService {

    private static final Logger LOG = LoggerFactory.getLogger(EthereumServiceImpl.class);

    private static final int DERIVATION_PATH_PURPOSE = 44;
    private static final int DERIVATION_PATH_COIN = 60;
    private static final int CHANGE_INDEX = 0;
    private static final int ADDRESS_INDEX = 0;

    private static final SecureRandom secureRandom = new SecureRandom();

    public Admin setUp(String URL) {
        if (URL.isEmpty()) {
            LOG.error("Infura URL is empty or incorrect. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.");
            throw new RuntimeException("Infura URL is empty or incorrect. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.");
        } else {
            if (!URL.contains("https://")) {
                URL = "https://" + URL;
            }
            return Admin.build(new HttpService(URL));
        }
    }

    private String getWeb3jURL(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        Environment environment = ApplicationContextProvider.getApplicationContext().getEnvironment();

        String URL;
        if (environment.getProperty("etherlinker.use.geth.or.parity").equals("true")) {
            URL = environment.getProperty("etherlinker.geth.or.parity.url");
        } else {

            if (etherlinkerRequestData.getInfuraURL().isEmpty() || !etherlinkerRequestData.getInfuraURL().contains("infura.io")) {
                throw new Exception("Infura URL is empty or incorrect. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.");
            }

            URL = etherlinkerRequestData.getInfuraURL();
        }

        if (!URL.contains("https://")) {
            URL = "https://" + URL;
        }

        return URL;
    }

    @Override
    public EtherlinkerResponseData transferEther(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        String receiverWalletAddress = etherlinkerRequestData.getReceiverAddress();
        if (!AddressUtils.IsAddressValid(receiverWalletAddress)) {
            throw new RestException("Invalid receiver address parameter");
        }
        receiverWalletAddress = Keys.toChecksumAddress(receiverWalletAddress);


        Admin web3j = setUp(getWeb3jURL(etherlinkerRequestData));
        try {
            Credentials wallet = loadCredentials(etherlinkerRequestData);

            EtherlinkerResponseData etherlinkerResponseData = new EtherlinkerResponseData();

            TransactionReceipt transactionReceipt = Transfer.sendFunds(
                    web3j, wallet, receiverWalletAddress,
                    new BigDecimal(etherlinkerRequestData.getEthAmountToSend()), Convert.Unit.ETHER).send();

            if (Optional.ofNullable(transactionReceipt).isPresent()) {
                etherlinkerResponseData.setData("Transaction " + transactionReceipt.getTransactionHash() + " successfully mined");
            } else {
                throw new Exception("Failed to get transaction receipt for transferEther method");
            }

            etherlinkerResponseData = enhanceResponseData(etherlinkerResponseData, etherlinkerRequestData, "transferEther");

            return etherlinkerResponseData;

        } finally {
            if (web3j != null) {
                web3j.shutdown();
            }
        }
    }

    @Override
    public EtherlinkerResponseData getBalance(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        String walletAddress = etherlinkerRequestData.getWalletAddress();
        if (!AddressUtils.IsAddressValid(walletAddress)) {
            throw new RestException("Invalid address parameter");
        }
        walletAddress = Keys.toChecksumAddress(walletAddress);

        Admin web3j = setUp(getWeb3jURL(etherlinkerRequestData));
        try {
            EtherlinkerResponseData etherlinkerResponseData = new EtherlinkerResponseData();

            EthGetBalance ethGetBalance = web3j
                    .ethGetBalance(walletAddress, DefaultBlockParameterName.LATEST)
                    .sendAsync()
                    .get();

            String balance;
            if (etherlinkerRequestData.getConvertResultFromWeiToEth().equals("true")) {
                balance = Convert.fromWei(ethGetBalance.getBalance().toString(), Convert.Unit.ETHER).toPlainString();
            } else {
                balance = ethGetBalance.getBalance().toString();
            }

            etherlinkerResponseData.setData(balance);

            etherlinkerResponseData = enhanceResponseData(etherlinkerResponseData, etherlinkerRequestData, "getWalletBalance");

            return etherlinkerResponseData;

        } finally {
            if (web3j != null) {
                web3j.shutdown();
            }
        }
    }

    @Override
    @SuppressWarnings("Duplicates")
    public EtherlinkerResponseData deployContract(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        Admin web3j = setUp(getWeb3jURL(etherlinkerRequestData));
        try {

            Credentials wallet = loadCredentials(etherlinkerRequestData);

            Class<?> contractClass = Class.forName(etherlinkerRequestData.getContractName());
            Method deployMethod = contractClass.getDeclaredMethod("deploy", Web3j.class, Credentials.class, BigInteger.class, BigInteger.class);

            BigInteger gasPrice = GAS_PRICE;
            BigInteger gasLimit = GAS_LIMIT;

            if (!etherlinkerRequestData.getGasPrice().isEmpty()) {
                gasPrice = new BigInteger(etherlinkerRequestData.getGasPrice());
            }

            if (!etherlinkerRequestData.getGasLimit().isEmpty()) {
                gasLimit = new BigInteger(etherlinkerRequestData.getGasLimit());
            }

            Object contract = ((RemoteCall<MethodParamsTester>) deployMethod.invoke(null, web3j, wallet, gasPrice, gasLimit)).send();

            Method getContractAddressMethod = contractClass.getSuperclass().getDeclaredMethod("getContractAddress");
            String contractAddress = (String) getContractAddressMethod.invoke((Contract) contract);
            LOG.info("Smart contract deployed to address " + contractAddress);

            EtherlinkerResponseData etherlinkerResponseData = new EtherlinkerResponseData();
            etherlinkerResponseData.setData("Smart contract deployed to address " + contractAddress);

            etherlinkerResponseData = enhanceResponseData(etherlinkerResponseData, etherlinkerRequestData, "deployContract");

            return etherlinkerResponseData;

        } finally {
            if (web3j != null) {
                web3j.shutdown();
            }
        }
    }

    @Override
    @SuppressWarnings("Duplicates")
    public EtherlinkerResponseData execContractMethod(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        String contractAddress = etherlinkerRequestData.getContractAddress();
        if (!AddressUtils.IsAddressValid(contractAddress)) {
            throw new RestException("Invalid contract address parameter");
        }
        contractAddress = Keys.toChecksumAddress(contractAddress);

        Admin web3j = setUp(getWeb3jURL(etherlinkerRequestData));
        try {
            Credentials wallet = loadCredentials(etherlinkerRequestData);

            Class<?> contractClass = Class.forName(etherlinkerRequestData.getContractName());
            Method loadMethod = contractClass.getDeclaredMethod("load", String.class, Web3j.class, Credentials.class, BigInteger.class, BigInteger.class);

            BigInteger gasPrice = GAS_PRICE;
            BigInteger gasLimit = GAS_LIMIT;

            if (!etherlinkerRequestData.getGasPrice().isEmpty()) {
                gasPrice = new BigInteger(etherlinkerRequestData.getGasPrice());
            }

            if (!etherlinkerRequestData.getGasLimit().isEmpty()) {
                gasLimit = new BigInteger(etherlinkerRequestData.getGasLimit());
            }

            Object contract = loadMethod.invoke(null, contractAddress, web3j, wallet, gasPrice, gasLimit);

            EtherlinkerResponseData etherlinkerResponseData = new EtherlinkerResponseData();

            // Process parameters and parameter types
            Object[] parameters = new Object[1];
            Class[] parameterTypes = new Class[1];
            if (etherlinkerRequestData.getContractMethodParams() != null && etherlinkerRequestData.getContractMethodParams().size() >= 0) {

                parameterTypes = new Class[etherlinkerRequestData.getContractMethodParamTypes().size()];
                for (int i = 0; i < etherlinkerRequestData.getContractMethodParamTypes().size(); i++) {
                    switch (etherlinkerRequestData.getContractMethodParamTypes().get(i)) {
                        case "String": {
                            parameterTypes[i] = String.class;
                            break;
                        }
                        case "Address": {
                            parameterTypes[i] = String.class;
                            break;
                        }
                        case "Number": {
                            parameterTypes[i] = BigInteger.class;
                            break;
                        }
                        case "ETH": {
                            parameterTypes[i] = BigInteger.class;
                            break;
                        }
                        case "Boolean": {
                            parameterTypes[i] = Boolean.class;
                            break;
                        }
                        case "Bytes": {
                            parameterTypes[i] = byte[].class;
                            break;
                        }
                        case "StringArray": {
                            parameterTypes[i] = List.class;
                            break;
                        }
                        case "BooleanArray": {
                            parameterTypes[i] = List.class;
                            break;
                        }
                        case "NumberArray": {
                            parameterTypes[i] = List.class;
                            break;
                        }
                        default: {
                            parameterTypes[i] = String.class;
                            break;
                        }
                    }
                }

                parameters = new Object[etherlinkerRequestData.getContractMethodParams().size()];
                for (int i = 0; i < etherlinkerRequestData.getContractMethodParams().size(); i++) {
                    switch (etherlinkerRequestData.getContractMethodParamTypes().get(i)) {
                        case "String": {
                            parameters[i] = etherlinkerRequestData.getContractMethodParams().get(i);
                            break;
                        }
                        case "Address": {
                            parameters[i] = etherlinkerRequestData.getContractMethodParams().get(i);
                            if (!AddressUtils.IsAddressValid((String) parameters[i])) {
                                throw new RestException("Invalid address parameter");
                            }
                            parameters[i] = Keys.toChecksumAddress((String) parameters[i]);
                            break;
                        }
                        case "Number": {
                            BigInteger numberParam;
                            if (etherlinkerRequestData.getContractMethodParams().get(i).contains("EtherToWei")) {
                                numberParam = Convert.toWei(etherlinkerRequestData.getContractMethodParams().get(i).split(" ")[0], Convert.Unit.ETHER).toBigIntegerExact();
                            } else {
                                numberParam = new BigInteger(etherlinkerRequestData.getContractMethodParams().get(i));
                            }

                            parameters[i] = numberParam;
                            break;
                        }
                        case "ETH": {
                            BigInteger numberParam = Convert.toWei(etherlinkerRequestData.getContractMethodParams().get(i), Convert.Unit.ETHER).toBigIntegerExact();
                            parameters[i] = numberParam;
                            break;
                        }
                        case "Boolean": {
                            parameters[i] = Boolean.parseBoolean(etherlinkerRequestData.getContractMethodParams().get(i));
                            break;
                        }
                        case "Bytes": {
                            List<String> stringArrayParams = new ArrayList<>(Arrays.asList(etherlinkerRequestData.getContractMethodParams().get(i).split(",")));
                            byte[] booleanArrayParams = new byte[stringArrayParams.size()];
                            for (int j = 0; j < stringArrayParams.size(); j++) {
                                booleanArrayParams[j] = Byte.parseByte(stringArrayParams.get(j).trim());
                            }
                            parameters[i] = booleanArrayParams;
                            break;
                        }
                        case "StringArray": {
                            // TODO: fix org.web3j.tx.exceptions.ContractCallException: Empty value (0x) returned from contract
                            List<String> stringArrayParams = new ArrayList<>(Arrays.asList(etherlinkerRequestData.getContractMethodParams().get(i).split(",")));
                            for (int j = 0; j < stringArrayParams.size(); j++) {
                                stringArrayParams.set(j, stringArrayParams.get(j).trim());
                            }
                            parameters[i] = stringArrayParams;
                            break;
                        }
                        case "BooleanArray": {
                            List<String> stringArrayParams = new ArrayList<>(Arrays.asList(etherlinkerRequestData.getContractMethodParams().get(i).split(",")));
                            List<Boolean> booleanArrayParams = new ArrayList<>();
                            for (String stringValue : stringArrayParams) {
                                booleanArrayParams.add(Boolean.parseBoolean(stringValue.trim()));
                            }
                            parameters[i] = booleanArrayParams;
                            break;
                        }
                        case "NumberArray": {
                            List<String> stringArrayParams = new ArrayList<>(Arrays.asList(etherlinkerRequestData.getContractMethodParams().get(i).split(",")));
                            List<BigInteger> numberArrayParams = new ArrayList<>();
                            for (String stringValue : stringArrayParams) {
                                try {
                                    numberArrayParams.add(new BigInteger(stringValue.trim()));
                                } catch (NumberFormatException nfe) {
                                    throw new RestException("Can't process number array input parameter. Some values aren't numbers.");
                                }
                            }
                            parameters[i] = numberArrayParams;
                            break;
                        }
                        default: {
                            parameters[i] = etherlinkerRequestData.getContractMethodParams().get(i);
                            break;
                        }
                    }
                }

            }

            // Send ethereum transaction and waiting for the result
            Method getNameMethod;
            Object result;
            if (etherlinkerRequestData.getContractMethodParams() == null || etherlinkerRequestData.getContractMethodParams().size() == 0) {
                getNameMethod = contract.getClass().getMethod(etherlinkerRequestData.getContractMethodName());
                result = ((RemoteCall) getNameMethod.invoke(contract)).send();
            } else {
                getNameMethod = contract.getClass().getMethod(etherlinkerRequestData.getContractMethodName(), parameterTypes);
                result = ((RemoteCall) getNameMethod.invoke(contract, parameters)).send();
            }

            // Process result
            if (result instanceof TransactionReceipt) {
                if (Optional.ofNullable(result).isPresent()) {
                    etherlinkerResponseData.setData("Transaction " + ((TransactionReceipt) result).getTransactionHash() + " successfully mined");
                } else {
                    throw new Exception("Failed to get transaction receipt for transferEther method");
                }
            } else if (result instanceof BigInteger && etherlinkerRequestData.getConvertResultFromWeiToEth().equals("true")) {
                etherlinkerResponseData.setData(Convert.fromWei(result.toString(), Convert.Unit.ETHER).toPlainString());
            } else if (result instanceof byte[]) {
                StringBuilder resultData = new StringBuilder();
                for (int i = 0; i < ((byte[]) result).length; i++) {
                    resultData.append(((byte[]) result)[i] + " ");
                }
                etherlinkerResponseData.setData(resultData.toString().trim());
            } else {
                etherlinkerResponseData.setData(result.toString());
            }

            // Enhance response with additional data
            etherlinkerResponseData = enhanceResponseData(etherlinkerResponseData, etherlinkerRequestData, "execContractMethod");

            return etherlinkerResponseData;

        } finally {
            if (web3j != null) {
                web3j.shutdown();
            }
        }

    }

    @Override
    public EtherlinkerBatchResponseData processBatchRequest(EtherlinkerBatchRequestData etherlinkerBatchRequestData) throws Exception {

        EtherlinkerBatchResponseData etherlinkerBatchResponseData = new EtherlinkerBatchResponseData();
        etherlinkerBatchResponseData.setEtherlinkerResponseDataList(new ArrayList<>());
        etherlinkerBatchResponseData.setSenderId(etherlinkerBatchRequestData.getSenderId());
        etherlinkerBatchResponseData.setUserIndex(etherlinkerBatchRequestData.getUserIndex());
        etherlinkerBatchResponseData.setData("Operation completed");
        etherlinkerBatchResponseData.setBatchResponseDataVersion("1");

        for (EtherlinkerRequestData etherlinkerRequestData : etherlinkerBatchRequestData.getEtherlinkerRequestDataList()) {
            switch (etherlinkerRequestData.getOperationType()) {
                case "getWalletBalance": {
                    etherlinkerBatchResponseData.getEtherlinkerResponseDataList().add(getBalance(etherlinkerRequestData));
                    break;
                }
                case "transferEther": {
                    etherlinkerBatchResponseData.getEtherlinkerResponseDataList().add(transferEther(etherlinkerRequestData));
                    break;
                }
                case "createWallet": {
                    etherlinkerBatchResponseData.getEtherlinkerResponseDataList().add(createWallet(etherlinkerRequestData));
                    break;
                }
                case "deployContract": {
                    etherlinkerBatchResponseData.getEtherlinkerResponseDataList().add(deployContract(etherlinkerRequestData));
                    break;
                }
                case "execContractMethod": {
                    etherlinkerBatchResponseData.getEtherlinkerResponseDataList().add(execContractMethod(etherlinkerRequestData));
                    break;
                }
            }
        }

        return etherlinkerBatchResponseData;
    }

    public ArrayList<String> createWallet(String path, String password) throws Exception {

        path = FilenameUtils.getFullPath(path);

        if (path == null || path.isEmpty()) {
            throw new Exception("Failed to get wallet path");
        }

        byte[] initialEntropy = new byte[16];
        secureRandom.nextBytes(initialEntropy);

        String mnemonic = MnemonicUtils.generateMnemonic(initialEntropy);
        DeterministicKey privateMasterKey = HDKeyDerivation.createMasterPrivateKey(MnemonicUtils.generateSeed(mnemonic, password));
        File walletFile = new File(path);
        if (!walletFile.exists()) {
            walletFile.mkdirs();
        }

        ArrayList<String> result = new ArrayList<>();

        String walletAddress = WalletUtils.generateWalletFile(password, ECKeyPair.create(fromBIP44HDpath(privateMasterKey, 0).getPrivKey()), walletFile, true);
        walletAddress = "0x" + walletAddress.split("--")[2].split("\\.")[0];
        result.add(walletAddress);
        result.add(mnemonic);

        return result;
    }

    @Override
    public EtherlinkerResponseData createWallet(EtherlinkerRequestData etherlinkerRequestData) throws Exception {

        EtherlinkerResponseData etherlinkerResponseData = new EtherlinkerResponseData();
        ArrayList<String> result = createWallet(etherlinkerRequestData.getWalletPath(), etherlinkerRequestData.getWalletPassword());
        StringBuilder builder = new StringBuilder();
        if (etherlinkerRequestData.getWalletPassword().isEmpty()) {
            builder.append(result.get(0)).append(";").append(result.get(1)).append(";").append("no password set");
        } else {
            builder.append(result.get(0)).append(";").append(result.get(1)).append(";").append(etherlinkerRequestData.getWalletPassword());
        }
        etherlinkerResponseData.setData("Wallet created. " + builder.toString());

        etherlinkerResponseData = enhanceResponseData(etherlinkerResponseData, etherlinkerRequestData, "createWallet");

        return etherlinkerResponseData;

    }

    public Credentials loadWallet(String password, String path) throws Exception {
        return WalletUtils.loadCredentials(password, path);
    }

    @Override
    public Credentials loadWalletBIP39(String password, String mnemonic) throws Exception {
        DeterministicKey privateMasterKey = HDKeyDerivation.createMasterPrivateKey(MnemonicUtils.generateSeed(mnemonic, password));
        return Credentials.create(ECKeyPair.create(fromBIP44HDpath(privateMasterKey, 0).getPrivKey()));
    }

    public static ECKey fromBIP44HDpath(DeterministicKey masterKey, int accountIndex) {
        DeterministicKey purposeKey = HDKeyDerivation.deriveChildKey(masterKey, DERIVATION_PATH_PURPOSE | ChildNumber.HARDENED_BIT);
        DeterministicKey rootKey = HDKeyDerivation.deriveChildKey(purposeKey, DERIVATION_PATH_COIN | ChildNumber.HARDENED_BIT);
        DeterministicKey accountKey = HDKeyDerivation.deriveChildKey(rootKey, accountIndex | ChildNumber.HARDENED_BIT);
        DeterministicKey changeKey = HDKeyDerivation.deriveChildKey(accountKey, CHANGE_INDEX);
        DeterministicKey addressKey = HDKeyDerivation.deriveChildKey(changeKey, ADDRESS_INDEX);
        return ECKey.fromPrivate(addressKey.getPrivKeyBytes());
    }

    @Override
    public EtherlinkerResponseData enhanceResponseData(EtherlinkerResponseData etherlinkerResponseData, EtherlinkerRequestData etherlinkerRequestData, String operationType) {

        etherlinkerResponseData.setContractAddress(etherlinkerRequestData.getContractAddress());
        etherlinkerResponseData.setContractName(etherlinkerRequestData.getContractName());
        etherlinkerResponseData.setContractMethodName(etherlinkerRequestData.getContractMethodName());
        etherlinkerResponseData.setWalletAddress(Keys.toChecksumAddress(etherlinkerRequestData.getWalletAddress()));
        etherlinkerResponseData.setOperationType(operationType);
        etherlinkerResponseData.setSenderId(etherlinkerRequestData.getSenderId());
        etherlinkerResponseData.setUserIndex(etherlinkerRequestData.getUserIndex());
        etherlinkerResponseData.setResponseDataVersion("1");

        return etherlinkerResponseData;
    }

    @Override
    public Credentials loadCredentials(EtherlinkerRequestData etherlinkerRequestData) throws Exception {
        switch (etherlinkerRequestData.getWalletAuthType()) {
            case "Mnemonic": {
                return loadWalletBIP39(etherlinkerRequestData.getWalletPassword(), etherlinkerRequestData.getWalletMnemonic());
            }
            case "PrivateKey": {
                return loadWallet(etherlinkerRequestData.getWalletPassword(), etherlinkerRequestData.getWalletPath());
            }
            default: {
                return loadWalletBIP39(etherlinkerRequestData.getWalletPassword(), etherlinkerRequestData.getWalletMnemonic());
            }
        }
    }
}
