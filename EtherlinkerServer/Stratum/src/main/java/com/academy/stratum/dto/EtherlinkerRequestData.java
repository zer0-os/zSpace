package com.academy.stratum.dto;

import java.util.ArrayList;

/**
 * Request data from compatible application (like UE4)
 *
 * @author Ruslan Nazirov
 */
public class EtherlinkerRequestData {

    /**
     * Unique sender identifier
     */
    private String senderId;

    /**
     * User index in a project with multiplayer support
     */
    private String userIndex;

    /**
     * Link to the Integration Server
     */
    private String serverAddress;

    /**
     * Address of the user wallet
     */
    private String walletAddress;

    /**
     * Password of the user wallet<br/>
     * Can be empty
     */
    private String walletPassword;

    /**
     * BIP39 mnemonic of the user wallet
     */
    private String walletMnemonic;

    /**
     * Path to the user wallet or directory, where wallets located
     */
    private String walletPath;

    /**
     * Authentication type.<br/>
     * Can be "Mnemonic" or "PrivateKey"
     */
    private String walletAuthType;

    /**
     * Address of the deployed solidity contract<br/>
     * Example: 0x5d260ae2d5014e450a70dc24be0f191353ec1a1e
     */
    private String contractAddress;

    /**
     * Method, which should be executed.<br/>
     * Example: stringTest
     */
    private String contractMethodName;

    /**
     * Contract name, which is the name of a Java class wrapper of solidity contract, which you will get after contract compilation.<br/>
     * Example: com.academy.contracts.MethodParamsTester
     */
    private String contractName;

    /**
     * Version of request data API<br/>
     * Current version: 1
     */
    private String requestDataVersion;

    /**
     * Allows you to receive results in user-friendly ETH format instead of long Wei format with a lot of digits.<br/>
     * It's recommended to set it true in transferEther, getWalletBalance operations and in execContractMethod if the method returns value in Wei format by default.<br/>
     */
    private String convertResultFromWeiToEth;

    /**
     * The higher the value, the faster your transactions will be processed.<br/>
     * The value should be in Wei format.<br/>
     * Default value: 22000000000
     */
    private String gasPrice;

    /**
     * The maximum amount of units of gas you are willing to spend on a transaction.<br/>
     * The default value is 4300000
     */
    private String gasLimit;

    /**
     * You should specify your custom access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.
     */
    private String InfuraURL;

    /**
     * Custom input parameters for the selected contract method.<br/>
     * Should be in the same order as in solidity contract.<br/>
     * If you have an array parameter, you should specify all its value in one parameter.
     */
    private ArrayList<String> contractMethodParams;

    /**
     * Types of custom input parameters for the selected contract method.<br/>
     * Should be in the same order as in solidity contract.
     */
    private ArrayList<String> contractMethodParamTypes;

    /**
     * Can be used with transferEther operationType to specify wallet on which you will send ETH.
     */
    private String receiverAddress;

    /**
     * Can be used with transferEther operationType in order to specify how much ETH you want to send.
     */
    private String ethAmountToSend;

    /**
     * Operation type, which was used during method execution (like "createWallet" or "deployContract")
     */
    private String operationType;

    public String getSenderId() {
        return senderId;
    }

    public void setSenderId(String senderId) {
        this.senderId = senderId;
    }

    public String getWalletAddress() {
        return walletAddress;
    }

    public void setWalletAddress(String walletAddress) {
        this.walletAddress = walletAddress;
    }

    public String getWalletPassword() {
        return walletPassword;
    }

    public void setWalletPassword(String walletPassword) {
        this.walletPassword = walletPassword;
    }

    public String getWalletPath() {
        return walletPath;
    }

    public void setWalletPath(String walletPath) {
        this.walletPath = walletPath;
    }

    public String getContractAddress() {
        return contractAddress;
    }

    public void setContractAddress(String contractAddress) {
        this.contractAddress = contractAddress;
    }

    public String getContractMethodName() {
        return contractMethodName;
    }

    public void setContractMethodName(String contractMethodName) {
        this.contractMethodName = contractMethodName;
    }

    public String getContractName() {
        return contractName;
    }

    public void setContractName(String contractName) {
        this.contractName = contractName;
    }

    public ArrayList<String> getContractMethodParams() {
        return contractMethodParams;
    }

    public String getWalletMnemonic() {
        return walletMnemonic;
    }

    public void setWalletMnemonic(String walletMnemonic) {
        this.walletMnemonic = walletMnemonic;
    }

    public String getWalletAuthType() {
        return walletAuthType;
    }

    public void setWalletAuthType(String walletAuthType) {
        this.walletAuthType = walletAuthType;
    }

    public void setContractMethodParams(ArrayList<String> contractMethodParams) {
        this.contractMethodParams = contractMethodParams;
    }

    public ArrayList<String> getContractMethodParamTypes() {
        return contractMethodParamTypes;
    }

    public void setContractMethodParamTypes(ArrayList<String> contractMethodParamTypes) {
        this.contractMethodParamTypes = contractMethodParamTypes;
    }

    public String getRequestDataVersion() {
        return requestDataVersion;
    }

    public void setRequestDataVersion(String requestDataVersion) {
        this.requestDataVersion = requestDataVersion;
    }

    public String getConvertResultFromWeiToEth() {
        return convertResultFromWeiToEth;
    }

    public void setConvertResultFromWeiToEth(String convertResultFromWeiToEth) {
        this.convertResultFromWeiToEth = convertResultFromWeiToEth;
    }

    public String getGasPrice() {
        return gasPrice;
    }

    public void setGasPrice(String gasPrice) {
        this.gasPrice = gasPrice;
    }

    public String getGasLimit() {
        return gasLimit;
    }

    public void setGasLimit(String gasLimit) {
        this.gasLimit = gasLimit;
    }

    public String getUserIndex() {
        return userIndex;
    }

    public void setUserIndex(String userIndex) {
        this.userIndex = userIndex;
    }

    public String getInfuraURL() {
        return InfuraURL;
    }

    public void setInfuraURL(String infuraURL) {
        this.InfuraURL = infuraURL;
    }

    public String getReceiverAddress() {
        return receiverAddress;
    }

    public void setReceiverAddress(String receiverAddress) {
        this.receiverAddress = receiverAddress;
    }

    public String getEthAmountToSend() {
        return ethAmountToSend;
    }

    public void setEthAmountToSend(String ethAmountToSend) {
        this.ethAmountToSend = ethAmountToSend;
    }

    public String getServerAddress() {
        return serverAddress;
    }

    public void setServerAddress(String serverAddress) {
        this.serverAddress = serverAddress;
    }

    public String getOperationType() {
        return operationType;
    }

    public void setOperationType(String operationType) {
        this.operationType = operationType;
    }
}
