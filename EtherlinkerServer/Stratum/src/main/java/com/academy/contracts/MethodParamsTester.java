package com.academy.contracts;

import java.math.BigInteger;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Callable;
import org.web3j.abi.TypeReference;
import org.web3j.abi.datatypes.Address;
import org.web3j.abi.datatypes.Bool;
import org.web3j.abi.datatypes.DynamicArray;
import org.web3j.abi.datatypes.DynamicBytes;
import org.web3j.abi.datatypes.Function;
import org.web3j.abi.datatypes.Type;
import org.web3j.abi.datatypes.Utf8String;
import org.web3j.abi.datatypes.generated.Bytes1;
import org.web3j.abi.datatypes.generated.Int256;
import org.web3j.abi.datatypes.generated.Int8;
import org.web3j.abi.datatypes.generated.Uint256;
import org.web3j.abi.datatypes.generated.Uint8;
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
public class MethodParamsTester extends Contract {
    public static final String BINARY = "608060405234801561001057600080fd5b50610b65806100206000396000f3fe608060405234801561001057600080fd5b50600436106100ea5760003560e01c806383d19ab41161008c578063985c4c4211610066578063985c4c42146101c5578063ac59c960146101e0578063cfae3217146101fb578063f225190714610203576100ea565b806383d19ab41461019757806392bd51fc146101b257806393bb62681461016e576100ea565b806352b696e4116100c857806352b696e4146101335780636a57db531461014e578063788e29f81461016e578063816fcff11461017c576100ea565b806306a2d5ac146100ef578063109cdfcd1461011857806322e51100146100ef575b600080fd5b6101026100fd36600461065f565b61021e565b60405161010f91906109f3565b60405180910390f35b6101266100fd366004610584565b60405161010f91906109b5565b6101416100fd3660046105ee565b60405161010f91906109c6565b61016161015c3660046105b9565b610225565b60405161010f91906109e2565b6101616100fd36600461062a565b61018a6100fd36600461054f565b60405161010f91906109a4565b6101a56100fd3660046106e2565b60405161010f9190610a01565b6101616101c036600461067d565b6102c1565b6101d36100fd366004610700565b60405161010f9190610a1f565b6101ee6100fd366004610529565b60405161010f9190610996565b61016161030f565b6102116100fd36600461060c565b60405161010f91906109d4565b805b919050565b606060405160200161023690610a0f565b604051602081830303815290604052805190602001208260008151811061025957fe5b602002602001015160405160200161027191906109e2565b6040516020818303038152906040528051906020012014156102b25760405160200161029c9061094e565b6040516020818303038152906040529050610220565b60405160200161029c90610975565b6060828413156102d2575080610308565b5060408051808201909152601981527f696e70757456616c756532203c20696e70757456616c7565310000000000000060208201525b9392505050565b60408051808201909152601a81527f48656c6c6f20457468657265756d20426c6f636b636861696e21000000000000602082015290565b60006103088235610ab0565b600082601f83011261036357600080fd5b813561037661037182610a54565b610a2d565b9150818183526020840193506020810190508385602084028201111561039b57600080fd5b60005b838110156103c757816103b1888261049e565b845250602092830192919091019060010161039e565b5050505092915050565b600082601f8301126103e257600080fd5b81356103f061037182610a54565b9150818183526020840193506020810190508385602084028201111561041557600080fd5b60005b838110156103c7578161042b8882610505565b8452506020928301929190910190600101610418565b600082601f83011261045257600080fd5b813561046061037182610a54565b81815260209384019390925082018360005b838110156103c7578135860161048888826104b6565b8452506020928301929190910190600101610472565b60006103088235610abb565b60006103088235610ac0565b600082601f8301126104c757600080fd5b81356104d561037182610a75565b915080825260208301602083018583830111156104f157600080fd5b6104fc838284610ae5565b50505092915050565b6000610308823561021e565b60006103088235610acd565b60006103088235610adf565b60006020828403121561053b57600080fd5b60006105478484610346565b949350505050565b60006020828403121561056157600080fd5b813567ffffffffffffffff81111561057857600080fd5b61054784828501610352565b60006020828403121561059657600080fd5b813567ffffffffffffffff8111156105ad57600080fd5b610547848285016103d1565b6000602082840312156105cb57600080fd5b813567ffffffffffffffff8111156105e257600080fd5b61054784828501610441565b60006020828403121561060057600080fd5b6000610547848461049e565b60006020828403121561061e57600080fd5b600061054784846104aa565b60006020828403121561063c57600080fd5b813567ffffffffffffffff81111561065357600080fd5b610547848285016104b6565b60006020828403121561067157600080fd5b60006105478484610505565b60008060006060848603121561069257600080fd5b600061069e8686610505565b93505060206106af86828701610505565b925050604084013567ffffffffffffffff8111156106cc57600080fd5b6106d8868287016104b6565b9150509250925092565b6000602082840312156106f457600080fd5b60006105478484610511565b60006020828403121561071257600080fd5b6000610547848461051d565b600061072a83836107ed565b505060200190565b600061072a8383610837565b61074781610ab0565b82525050565b600061075882610aa3565b6107628185610aa7565b935061076d83610a9d565b60005b828110156107985761078386835161071e565b955061078e82610a9d565b9150600101610770565b5093949350505050565b60006107ad82610aa3565b6107b78185610aa7565b93506107c283610a9d565b60005b82811015610798576107d8868351610732565b95506107e382610a9d565b91506001016107c5565b61074781610abb565b61074781610ac0565b600061080a82610aa3565b6108148185610aa7565b9350610824818560208601610af1565b61082d81610b21565b9093019392505050565b6107478161021e565b61074781610acd565b6000610856600583610220565b600160d81b64746573743302815260050192915050565b600061087a600283610aa7565b600160f01b61413102815260200192915050565b600061089b600583610220565b600160d91b643a32b9ba1902815260050192915050565b60006108bf600483610220565b600160e21b6315d95b9d02815260040192915050565b60006108e2600583610220565b600160d81b64746573743102815260050192915050565b6000610906600583610220565b600160d81b6457726f6e6702815260050192915050565b600061092a600983610220565b600160b81b68536f6d657468696e6702815260090192915050565b61074781610adf565b6000610959826108d5565b91506109648261088e565b915061096f82610849565b92915050565b60006109808261091d565b915061098b826108b2565b915061096f826108f9565b6020810161096f828461073e565b60208082528101610308818461074d565b6020808252810161030881846107a2565b6020810161096f82846107ed565b6020810161096f82846107f6565b6020808252810161030881846107ff565b6020810161096f8284610837565b6020810161096f8284610840565b6020808252810161096f8161086d565b6020810161096f8284610945565b60405181810167ffffffffffffffff81118282101715610a4c57600080fd5b604052919050565b600067ffffffffffffffff821115610a6b57600080fd5b5060209081020190565b600067ffffffffffffffff821115610a8c57600080fd5b506020601f91909101601f19160190565b60200190565b5190565b90815260200190565b600061096f82610ad3565b151590565b6001600160f81b03191690565b60000b90565b6001600160a01b031690565b60ff1690565b82818337506000910152565b60005b83811015610b0c578181015183820152602001610af4565b83811115610b1b576000848401525b50505050565b601f01601f19169056fea265627a7a7230582053cc5a4df1557147e46207ceb68844fad14748efa28a0723ae3a5388c3a5bbe26c6578706572696d656e74616cf50037";

    public static final String FUNC_INTTEST = "intTest";

    public static final String FUNC_INTARRAYTEST = "intArrayTest";

    public static final String FUNC_UINTTEST = "uintTest";

    public static final String FUNC_BOOLTEST = "boolTest";

    public static final String FUNC_STRINGARRAYTEST = "stringArrayTest";

    public static final String FUNC_STRINGTEST = "stringTest";

    public static final String FUNC_BOOLARRAYTEST = "boolArrayTest";

    public static final String FUNC_INT8TEST = "int8Test";

    public static final String FUNC_MULTIPLEPARAMSTEST = "multipleParamsTest";

    public static final String FUNC_BYTESARRAYTEST = "bytesArrayTest";

    public static final String FUNC_UINT8TEST = "uint8Test";

    public static final String FUNC_ADDRESSTEST = "addressTest";

    public static final String FUNC_GREET = "greet";

    public static final String FUNC_BYTETEST = "byteTest";

    @Deprecated
    protected MethodParamsTester(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected MethodParamsTester(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected MethodParamsTester(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected MethodParamsTester(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<BigInteger> intTest(BigInteger inputValue) {
        final Function function = new Function(FUNC_INTTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Int256(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Int256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<List> intArrayTest(List<BigInteger> inputValue) {
        final Function function = new Function(FUNC_INTARRAYTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Int256>(
                        org.web3j.abi.datatypes.generated.Int256.class,
                        org.web3j.abi.Utils.typeMap(inputValue, org.web3j.abi.datatypes.generated.Int256.class))), 
                Arrays.<TypeReference<?>>asList(new TypeReference<DynamicArray<Int256>>() {}));
        return new RemoteFunctionCall<List>(function,
                new Callable<List>() {
                    @Override
                    @SuppressWarnings("unchecked")
                    public List call() throws Exception {
                        List<Type> result = (List<Type>) executeCallSingleValueReturn(function, List.class);
                        return convertToNative(result);
                    }
                });
    }

    public RemoteFunctionCall<BigInteger> uintTest(BigInteger inputValue) {
        final Function function = new Function(FUNC_UINTTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<Boolean> boolTest(Boolean inputValue) {
        final Function function = new Function(FUNC_BOOLTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Bool(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<String> stringArrayTest(List<String> inputValue) {
        final Function function = new Function(FUNC_STRINGARRAYTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.Utf8String>(
                        org.web3j.abi.datatypes.Utf8String.class,
                        org.web3j.abi.Utils.typeMap(inputValue, org.web3j.abi.datatypes.Utf8String.class))), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<String> stringTest(String inputValue) {
        final Function function = new Function(FUNC_STRINGTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<List> boolArrayTest(List<Boolean> inputValue) {
        final Function function = new Function(FUNC_BOOLARRAYTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.Bool>(
                        org.web3j.abi.datatypes.Bool.class,
                        org.web3j.abi.Utils.typeMap(inputValue, org.web3j.abi.datatypes.Bool.class))), 
                Arrays.<TypeReference<?>>asList(new TypeReference<DynamicArray<Bool>>() {}));
        return new RemoteFunctionCall<List>(function,
                new Callable<List>() {
                    @Override
                    @SuppressWarnings("unchecked")
                    public List call() throws Exception {
                        List<Type> result = (List<Type>) executeCallSingleValueReturn(function, List.class);
                        return convertToNative(result);
                    }
                });
    }

    public RemoteFunctionCall<BigInteger> int8Test(BigInteger inputValue) {
        final Function function = new Function(FUNC_INT8TEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Int8(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Int8>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<String> multipleParamsTest(BigInteger inputValue1, BigInteger inputValue2, String inputValue3) {
        final Function function = new Function(FUNC_MULTIPLEPARAMSTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Int256(inputValue1), 
                new org.web3j.abi.datatypes.generated.Int256(inputValue2), 
                new org.web3j.abi.datatypes.Utf8String(inputValue3)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<byte[]> bytesArrayTest(byte[] inputValue) {
        final Function function = new Function(FUNC_BYTESARRAYTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicBytes(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<DynamicBytes>() {}));
        return executeRemoteCallSingleValueReturn(function, byte[].class);
    }

    public RemoteFunctionCall<BigInteger> uint8Test(BigInteger inputValue) {
        final Function function = new Function(FUNC_UINT8TEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint8(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint8>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<String> addressTest(String inputValue) {
        final Function function = new Function(FUNC_ADDRESSTEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Address>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<String> greet() {
        final Function function = new Function(FUNC_GREET, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<byte[]> byteTest(byte[] inputValue) {
        final Function function = new Function(FUNC_BYTETEST, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Bytes1(inputValue)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bytes1>() {}));
        return executeRemoteCallSingleValueReturn(function, byte[].class);
    }

    @Deprecated
    public static MethodParamsTester load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new MethodParamsTester(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static MethodParamsTester load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new MethodParamsTester(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static MethodParamsTester load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new MethodParamsTester(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static MethodParamsTester load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new MethodParamsTester(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<MethodParamsTester> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(MethodParamsTester.class, web3j, credentials, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<MethodParamsTester> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(MethodParamsTester.class, web3j, credentials, gasPrice, gasLimit, BINARY, "");
    }

    public static RemoteCall<MethodParamsTester> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(MethodParamsTester.class, web3j, transactionManager, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<MethodParamsTester> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(MethodParamsTester.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, "");
    }
}
