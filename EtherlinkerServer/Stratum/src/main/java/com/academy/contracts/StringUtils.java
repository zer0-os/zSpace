package com.academy.contracts;

import java.math.BigInteger;
import java.util.Arrays;
import org.web3j.abi.TypeReference;
import org.web3j.abi.datatypes.Function;
import org.web3j.abi.datatypes.Type;
import org.web3j.abi.datatypes.Utf8String;
import org.web3j.abi.datatypes.generated.Bytes32;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.core.RemoteCall;
import org.web3j.protocol.core.RemoteFunctionCall;
import org.web3j.tx.Contract;
import org.web3j.tx.TransactionManager;
import org.web3j.tx.gas.ContractGasProvider;

/**
 * <p>Auto generated code.
 * <p><strong>Do not modify!</strong>
 * <p>Please use the <a href="https://docs.web3j.io/command_line.html">web3j command line tools</a>,
 * or the org.web3j.codegen.SolidityFunctionWrapperGenerator in the 
 * <a href="https://github.com/web3j/web3j/tree/master/codegen">codegen module</a> to update.
 *
 * <p>Generated with web3j version 4.5.11.
 */
@SuppressWarnings("rawtypes")
public class StringUtils extends Contract {
    public static final String BINARY = "608060405234801561001057600080fd5b506102bb806100206000396000f3fe608060405234801561001057600080fd5b50600436106100415760003560e01c80639201de551461004657806394e8767d146100d8578063e939567914610107575b600080fd5b6100636004803603602081101561005c57600080fd5b5035610124565b6040805160208082528351818301528351919283929083019185019080838360005b8381101561009d578181015183820152602001610085565b50505050905090810190601f1680156100ca5780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b6100f5600480360360208110156100ee57600080fd5b5035610229565b60408051918252519081900360200190f35b6100636004803603602081101561011d57600080fd5b5035610276565b6040805160208082528183019092526060918291906020820181803883390190505090506000805b60208110156101a2576008810260020a85026001600160f81b0319811615610199578084848151811061017b57fe5b60200101906001600160f81b031916908160001a9053506001909201915b5060010161014c565b506060816040519080825280601f01601f1916602001820160405280156101d0576020820181803883390190505b50905060005b82811015610220578381815181106101ea57fe5b602001015160f81c60f81b82828151811061020157fe5b60200101906001600160f81b031916908160001a9053506001016101d6565b50949350505050565b60008161023e5750600160fc1b600302610271565b81156102715761010081049050600a8206603001600160f81b0260001b81179050600a828161026957fe5b04915061023e565b919050565b606061028961028483610229565b610124565b9291505056fea165627a7a723058209530d5eda2a56b0033cd4a7a8119afc125ad986d2b9bcca6d1dc172534754b3d0029";

    public static final String FUNC_BYTES32TOSTRING = "bytes32ToString";

    public static final String FUNC_UINTTOBYTES = "uintToBytes";

    public static final String FUNC_UINTTOSTRING = "uintToString";

    @Deprecated
    protected StringUtils(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected StringUtils(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected StringUtils(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected StringUtils(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<String> bytes32ToString(byte[] x) {
        final Function function = new Function(FUNC_BYTES32TOSTRING, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Bytes32(x)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<byte[]> uintToBytes(BigInteger v) {
        final Function function = new Function(FUNC_UINTTOBYTES, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(v)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bytes32>() {}));
        return executeRemoteCallSingleValueReturn(function, byte[].class);
    }

    public RemoteFunctionCall<String> uintToString(BigInteger v) {
        final Function function = new Function(FUNC_UINTTOSTRING, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(v)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    @Deprecated
    public static StringUtils load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new StringUtils(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static StringUtils load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new StringUtils(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static StringUtils load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new StringUtils(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static StringUtils load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new StringUtils(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<StringUtils> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(StringUtils.class, web3j, credentials, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<StringUtils> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(StringUtils.class, web3j, credentials, gasPrice, gasLimit, BINARY, "");
    }

    public static RemoteCall<StringUtils> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(StringUtils.class, web3j, transactionManager, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<StringUtils> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(StringUtils.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, "");
    }
}
