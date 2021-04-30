package com.academy.contracts;

import io.reactivex.Flowable;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import org.web3j.abi.EventEncoder;
import org.web3j.abi.TypeReference;
import org.web3j.abi.datatypes.Address;
import org.web3j.abi.datatypes.Bool;
import org.web3j.abi.datatypes.Event;
import org.web3j.abi.datatypes.Function;
import org.web3j.abi.datatypes.Type;
import org.web3j.abi.datatypes.generated.Uint256;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.core.DefaultBlockParameter;
import org.web3j.protocol.core.RemoteCall;
import org.web3j.protocol.core.RemoteFunctionCall;
import org.web3j.protocol.core.methods.request.EthFilter;
import org.web3j.protocol.core.methods.response.BaseEventResponse;
import org.web3j.protocol.core.methods.response.Log;
import org.web3j.protocol.core.methods.response.TransactionReceipt;
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
public class PremiumItemToken extends Contract {
    public static final String BINARY = "6080604052600061001461006760201b60201c565b600380546001600160a01b0319166001600160a01b038316908117909155604051919250906000907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0908290a35061006b565b3390565b6118708061007a6000396000f3fe608060405234801561001057600080fd5b50600436106101735760003560e01c8063761232e0116100de578063a9059cbb11610097578063dd62ed3e11610071578063dd62ed3e1461058d578063f2fde38b146105bb578063f3cb5438146105e1578063f7d26c84146105fe57610173565b8063a9059cbb14610518578063bb714a1f14610544578063d40a557f1461057057610173565b8063761232e01461033457806379cc6790146103f25780637e5531561461041e5780638da5cb5b146104dc5780638f32d59b146104e4578063a457c2d7146104ec57610173565b806342966c681161013057806342966c681461027c57806348b68140146102995780634faf180c146102bd5780636af3bfce146102da57806370a0823114610306578063715018a61461032c57610173565b8063095ea7b31461017857806318160ddd146101b857806323b872dd146101d257806324d9dcad14610208578063395093511461022d5780633a2dc67914610259575b600080fd5b6101a46004803603604081101561018e57600080fd5b506001600160a01b03813516906020013561066c565b604080519115158252519081900360200190f35b6101c0610689565b60408051918252519081900360200190f35b6101a4600480360360608110156101e857600080fd5b506001600160a01b0381358116916020810135909116906040013561068f565b61022b6004803603604081101561021e57600080fd5b508035906020013561071c565b005b6101a46004803603604081101561024357600080fd5b506001600160a01b0381351690602001356107d4565b61022b6004803603604081101561026f57600080fd5b5080359060200135610828565b61022b6004803603602081101561029257600080fd5b50356108e0565b6102a16108f4565b604080516001600160a01b039092168252519081900360200190f35b61022b600480360360208110156102d357600080fd5b5035610950565b6101a4600480360360408110156102f057600080fd5b506001600160a01b038135169060200135610a4f565b6101c06004803603602081101561031c57600080fd5b50356001600160a01b0316610a6e565b61022b610a89565b61022b6004803603604081101561034a57600080fd5b810190602081018135600160201b81111561036457600080fd5b82018360208201111561037657600080fd5b803590602001918460208302840111600160201b8311171561039757600080fd5b919390929091602081019035600160201b8111156103b457600080fd5b8201836020820111156103c657600080fd5b803590602001918460208302840111600160201b831117156103e757600080fd5b509092509050610b1d565b61022b6004803603604081101561040857600080fd5b506001600160a01b038135169060200135610c52565b61022b6004803603604081101561043457600080fd5b810190602081018135600160201b81111561044e57600080fd5b82018360208201111561046057600080fd5b803590602001918460208302840111600160201b8311171561048157600080fd5b919390929091602081019035600160201b81111561049e57600080fd5b8201836020820111156104b057600080fd5b803590602001918460208302840111600160201b831117156104d157600080fd5b509092509050610c60565b6102a1610d8e565b6101a4610d9d565b6101a46004803603604081101561050257600080fd5b506001600160a01b038135169060200135610dc3565b6101a46004803603604081101561052e57600080fd5b506001600160a01b038135169060200135610e31565b61022b6004803603604081101561055a57600080fd5b506001600160a01b038135169060200135610e45565b6101c06004803603602081101561058657600080fd5b5035610ec3565b6101c0600480360360408110156105a357600080fd5b506001600160a01b0381358116916020013516610ed8565b61022b600480360360208110156105d157600080fd5b50356001600160a01b0316610f03565b61022b600480360360208110156105f757600080fd5b5035610f56565b61022b6004803603602081101561061457600080fd5b810190602081018135600160201b81111561062e57600080fd5b82018360208201111561064057600080fd5b803590602001918460208302840111600160201b8311171561066157600080fd5b509092509050610fef565b60006106806106796110cc565b84846110d0565b50600192915050565b60025490565b600061069c8484846111c2565b610712846106a86110cc565b61070d8560405180606001604052806028815260200161174a602891396001600160a01b038a166000908152600160205260408120906106e66110cc565b6001600160a01b03168152602081019190915260400160002054919063ffffffff61132416565b6110d0565b5060019392505050565b610724610d9d565b6107665760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b61076e61165b565b5060408051808201825283815260208082018481526000868152600583528490208351815590516001909101558251858152925191927f5f9c2ea8b2ea07dceb39996762bb28af4cc5ed42e4b4ac84c3432bcb9ae06368929081900390910190a1505050565b60006106806107e16110cc565b8461070d85600160006107f26110cc565b6001600160a01b03908116825260208083019390935260409182016000908120918c16815292529020549063ffffffff6113be16565b610830610d9d565b6108725760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b61087a61165b565b5060408051808201825283815260208082018481526000868152600583528490208351815590516001909101558251858152925191927f324840da1b07686a48ba5e392904b4ce809de98ec4a720311da7286d913cdf79929081900390910190a1505050565b6108f16108eb6110cc565b82611422565b50565b60006108fe610d9d565b6109405760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b506003546001600160a01b031690565b60008181526005602052604090205461096857600080fd5b60008181526005602052604090206001015461098333610a6e565b10156109d95760408051600160e51b62461bcd02815260206004820152601960248201527f4572726f723a20696e73756666696369656e742066756e647300000000000000604482015290519081900360640190fd5b6109e33382610a4f565b15610a2257604051600160e51b62461bcd0281526004018080602001828103825260218152602001806116bb6021913960400191505060405180910390fd5b600081815260056020526040902060010154610a3d906108e0565b33600090815260046020526040902055565b6001600160a01b03919091166000908152600460205260409020541490565b6001600160a01b031660009081526020819052604090205490565b610a91610d9d565b610ad35760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b6003546040516000916001600160a01b0316907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0908390a3600380546001600160a01b0319169055565b610b25610d9d565b610b675760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b828114610b7357600080fd5b60005b83811015610c4b57610b8661165b565b6040518060400160405280878785818110610b9d57fe5b905060200201358152602001858585818110610bb557fe5b9050602002013581525090508060056000888886818110610bd257fe5b6020908102929092013583525081810192909252604001600020825181559101516001909101557f324840da1b07686a48ba5e392904b4ce809de98ec4a720311da7286d913cdf79868684818110610c2657fe5b905060200201356040518082815260200191505060405180910390a150600101610b76565b5050505050565b610c5c8282611521565b5050565b610c68610d9d565b610caa5760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b828114610cb657600080fd5b60005b83811015610c4b57610cc961165b565b6040518060400160405280878785818110610ce057fe5b905060200201358152602001858585818110610cf857fe5b9050602002013581525090508060056000888886818110610d1557fe5b6020908102929092013583525081810192909252604001600020825181559101516001909101557f5f9c2ea8b2ea07dceb39996762bb28af4cc5ed42e4b4ac84c3432bcb9ae06368868684818110610d6957fe5b905060200201356040518082815260200191505060405180910390a150600101610cb9565b6003546001600160a01b031690565b6003546000906001600160a01b0316610db46110cc565b6001600160a01b031614905090565b6000610680610dd06110cc565b8461070d856040518060600160405280602581526020016118206025913960016000610dfa6110cc565b6001600160a01b03908116825260208083019390935260409182016000908120918d1681529252902054919063ffffffff61132416565b6000610680610e3e6110cc565b84846111c2565b610e4d610d9d565b610e8f5760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b600081815260056020526040902054610ea757600080fd5b6001600160a01b03909116600090815260046020526040902055565b60009081526005602052604090206001015490565b6001600160a01b03918216600090815260016020908152604080832093909416825291909152205490565b610f0b610d9d565b610f4d5760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b6108f181611575565b610f5e610d9d565b610fa05760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b600081815260056020908152604080832083815560010192909255815183815291517f0e37a5803a75db34b651d4e8b54e6454320dc026c797d41dc7e136b0033582bc9281900390910190a150565b610ff7610d9d565b6110395760408051600160e51b62461bcd0281526020600482018190526024820152600080516020611772833981519152604482015290519081900360640190fd5b60005b818110156110c7576005600084848481811061105457fe5b602090810292909201358352508101919091526040016000908120818155600101557f0e37a5803a75db34b651d4e8b54e6454320dc026c797d41dc7e136b0033582bc8383838181106110a357fe5b905060200201356040518082815260200191505060405180910390a160010161103c565b505050565b3390565b6001600160a01b03831661111857604051600160e51b62461bcd0281526004018080602001828103825260248152602001806117fc6024913960400191505060405180910390fd5b6001600160a01b03821661116057604051600160e51b62461bcd0281526004018080602001828103825260228152602001806117026022913960400191505060405180910390fd5b6001600160a01b03808416600081815260016020908152604080832094871680845294825291829020859055815185815291517f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b9259281900390910190a3505050565b6001600160a01b03831661120a57604051600160e51b62461bcd0281526004018080602001828103825260258152602001806117d76025913960400191505060405180910390fd5b6001600160a01b03821661125257604051600160e51b62461bcd0281526004018080602001828103825260238152602001806116766023913960400191505060405180910390fd5b61129581604051806060016040528060268152602001611724602691396001600160a01b038616600090815260208190526040902054919063ffffffff61132416565b6001600160a01b0380851660009081526020819052604080822093909355908416815220546112ca908263ffffffff6113be16565b6001600160a01b038084166000818152602081815260409182902094909455805185815290519193928716927fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef92918290030190a3505050565b600081848411156113b657604051600160e51b62461bcd0281526004018080602001828103825283818151815260200191508051906020019080838360005b8381101561137b578181015183820152602001611363565b50505050905090810190601f1680156113a85780820380516001836020036101000a031916815260200191505b509250505060405180910390fd5b505050900390565b60008282018381101561141b5760408051600160e51b62461bcd02815260206004820152601b60248201527f536166654d6174683a206164646974696f6e206f766572666c6f770000000000604482015290519081900360640190fd5b9392505050565b6001600160a01b03821661146a57604051600160e51b62461bcd0281526004018080602001828103825260218152602001806117b66021913960400191505060405180910390fd5b6114ad81604051806060016040528060228152602001611699602291396001600160a01b038516600090815260208190526040902054919063ffffffff61132416565b6001600160a01b0383166000908152602081905260409020556002546114d9908263ffffffff61161916565b6002556040805182815290516000916001600160a01b038516917fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef9181900360200190a35050565b61152b8282611422565b610c5c826115376110cc565b61070d84604051806060016040528060248152602001611792602491396001600160a01b0388166000908152600160205260408120906106e66110cc565b6001600160a01b0381166115bd57604051600160e51b62461bcd0281526004018080602001828103825260268152602001806116dc6026913960400191505060405180910390fd5b6003546040516001600160a01b038084169216907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e090600090a3600380546001600160a01b0319166001600160a01b0392909216919091179055565b600061141b83836040518060400160405280601e81526020017f536166654d6174683a207375627472616374696f6e206f766572666c6f770000815250611324565b60405180604001604052806000815260200160008152509056fe45524332303a207472616e7366657220746f20746865207a65726f206164647265737345524332303a206275726e20616d6f756e7420657863656564732062616c616e63654572726f723a207072656d69756d206974656d20616c7265616479206f776e65644f776e61626c653a206e6577206f776e657220697320746865207a65726f206164647265737345524332303a20617070726f766520746f20746865207a65726f206164647265737345524332303a207472616e7366657220616d6f756e7420657863656564732062616c616e636545524332303a207472616e7366657220616d6f756e74206578636565647320616c6c6f77616e63654f776e61626c653a2063616c6c6572206973206e6f7420746865206f776e657245524332303a206275726e20616d6f756e74206578636565647320616c6c6f77616e636545524332303a206275726e2066726f6d20746865207a65726f206164647265737345524332303a207472616e736665722066726f6d20746865207a65726f206164647265737345524332303a20617070726f76652066726f6d20746865207a65726f206164647265737345524332303a2064656372656173656420616c6c6f77616e63652062656c6f77207a65726fa165627a7a723058206b049cc7210a804dfdcf3d4d9a68d4a37d32d527eb82c68937b2ff2b561974320029";

    public static final String FUNC_APPROVE = "approve";

    public static final String FUNC_TOTALSUPPLY = "totalSupply";

    public static final String FUNC_TRANSFERFROM = "transferFrom";

    public static final String FUNC_EDITPREMIUMITEM = "editPremiumItem";

    public static final String FUNC_INCREASEALLOWANCE = "increaseAllowance";

    public static final String FUNC_ADDPREMIUMITEM = "addPremiumItem";

    public static final String FUNC_BURN = "burn";

    public static final String FUNC_OWNERPAYABLE = "ownerPayable";

    public static final String FUNC_BUYPREMIUMITEM = "buyPremiumItem";

    public static final String FUNC_CHECKPREMIUMITEM = "checkPremiumItem";

    public static final String FUNC_BALANCEOF = "balanceOf";

    public static final String FUNC_RENOUNCEOWNERSHIP = "renounceOwnership";

    public static final String FUNC_BATCHADDPREMIUMITEMS = "batchAddPremiumItems";

    public static final String FUNC_BURNFROM = "burnFrom";

    public static final String FUNC_BATCHEDITPREMIUMITEMS = "batchEditPremiumItems";

    public static final String FUNC_OWNER = "owner";

    public static final String FUNC_ISOWNER = "isOwner";

    public static final String FUNC_DECREASEALLOWANCE = "decreaseAllowance";

    public static final String FUNC_TRANSFER = "transfer";

    public static final String FUNC_GRANTPREMIUMITEM = "grantPremiumItem";

    public static final String FUNC_GETPREMIUMITEMPRICE = "getPremiumItemPrice";

    public static final String FUNC_ALLOWANCE = "allowance";

    public static final String FUNC_TRANSFEROWNERSHIP = "transferOwnership";

    public static final String FUNC_DELETEPREMIUMITEM = "deletePremiumItem";

    public static final String FUNC_BATCHDELETEPREMIUMITEMS = "batchDeletePremiumItems";

    public static final Event ADDPREMIUMITEMEVENT_EVENT = new Event("AddPremiumItemEvent", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
    ;

    public static final Event EDITPREMIUMITEMEVENT_EVENT = new Event("EditPremiumItemEvent", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
    ;

    public static final Event DELETEPREMIUMITEMEVENT_EVENT = new Event("DeletePremiumItemEvent", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
    ;

    public static final Event OWNERSHIPTRANSFERRED_EVENT = new Event("OwnershipTransferred", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}));
    ;

    public static final Event TRANSFER_EVENT = new Event("Transfer", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}, new TypeReference<Uint256>() {}));
    ;

    public static final Event APPROVAL_EVENT = new Event("Approval", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}, new TypeReference<Uint256>() {}));
    ;

    @Deprecated
    protected PremiumItemToken(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected PremiumItemToken(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected PremiumItemToken(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected PremiumItemToken(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<TransactionReceipt> approve(String spender, BigInteger amount) {
        final Function function = new Function(
                FUNC_APPROVE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, spender), 
                new org.web3j.abi.datatypes.generated.Uint256(amount)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<BigInteger> totalSupply() {
        final Function function = new Function(FUNC_TOTALSUPPLY, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> transferFrom(String sender, String recipient, BigInteger amount) {
        final Function function = new Function(
                FUNC_TRANSFERFROM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, sender), 
                new org.web3j.abi.datatypes.Address(160, recipient), 
                new org.web3j.abi.datatypes.generated.Uint256(amount)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> editPremiumItem(BigInteger _itemId, BigInteger _itemPrice) {
        final Function function = new Function(
                FUNC_EDITPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_itemId), 
                new org.web3j.abi.datatypes.generated.Uint256(_itemPrice)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> increaseAllowance(String spender, BigInteger addedValue) {
        final Function function = new Function(
                FUNC_INCREASEALLOWANCE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, spender), 
                new org.web3j.abi.datatypes.generated.Uint256(addedValue)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> addPremiumItem(BigInteger _itemId, BigInteger _itemPrice) {
        final Function function = new Function(
                FUNC_ADDPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_itemId), 
                new org.web3j.abi.datatypes.generated.Uint256(_itemPrice)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> burn(BigInteger amount) {
        final Function function = new Function(
                FUNC_BURN, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(amount)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> ownerPayable() {
        final Function function = new Function(FUNC_OWNERPAYABLE, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Address>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<TransactionReceipt> buyPremiumItem(BigInteger _itemId) {
        final Function function = new Function(
                FUNC_BUYPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_itemId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<Boolean> checkPremiumItem(String _user, BigInteger _itemId) {
        final Function function = new Function(FUNC_CHECKPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, _user), 
                new org.web3j.abi.datatypes.generated.Uint256(_itemId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<BigInteger> balanceOf(String account) {
        final Function function = new Function(FUNC_BALANCEOF, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, account)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> renounceOwnership() {
        final Function function = new Function(
                FUNC_RENOUNCEOWNERSHIP, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> batchAddPremiumItems(List<BigInteger> _itemIds, List<BigInteger> _itemPrices) {
        final Function function = new Function(
                FUNC_BATCHADDPREMIUMITEMS, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_itemIds, org.web3j.abi.datatypes.generated.Uint256.class)), 
                new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_itemPrices, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> burnFrom(String account, BigInteger amount) {
        final Function function = new Function(
                FUNC_BURNFROM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, account), 
                new org.web3j.abi.datatypes.generated.Uint256(amount)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> batchEditPremiumItems(List<BigInteger> _itemIds, List<BigInteger> _itemPrices) {
        final Function function = new Function(
                FUNC_BATCHEDITPREMIUMITEMS, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_itemIds, org.web3j.abi.datatypes.generated.Uint256.class)), 
                new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_itemPrices, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> owner() {
        final Function function = new Function(FUNC_OWNER, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Address>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<Boolean> isOwner() {
        final Function function = new Function(FUNC_ISOWNER, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<TransactionReceipt> decreaseAllowance(String spender, BigInteger subtractedValue) {
        final Function function = new Function(
                FUNC_DECREASEALLOWANCE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, spender), 
                new org.web3j.abi.datatypes.generated.Uint256(subtractedValue)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> transfer(String recipient, BigInteger amount) {
        final Function function = new Function(
                FUNC_TRANSFER, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, recipient), 
                new org.web3j.abi.datatypes.generated.Uint256(amount)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> grantPremiumItem(String _user, BigInteger _itemId) {
        final Function function = new Function(
                FUNC_GRANTPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, _user), 
                new org.web3j.abi.datatypes.generated.Uint256(_itemId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<BigInteger> getPremiumItemPrice(BigInteger _itemId) {
        final Function function = new Function(FUNC_GETPREMIUMITEMPRICE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_itemId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<BigInteger> allowance(String owner, String spender) {
        final Function function = new Function(FUNC_ALLOWANCE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, owner), 
                new org.web3j.abi.datatypes.Address(160, spender)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> transferOwnership(String newOwner) {
        final Function function = new Function(
                FUNC_TRANSFEROWNERSHIP, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, newOwner)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> deletePremiumItem(BigInteger _itemId) {
        final Function function = new Function(
                FUNC_DELETEPREMIUMITEM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_itemId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> batchDeletePremiumItems(List<BigInteger> _itemIds) {
        final Function function = new Function(
                FUNC_BATCHDELETEPREMIUMITEMS, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_itemIds, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public List<AddPremiumItemEventEventResponse> getAddPremiumItemEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(ADDPREMIUMITEMEVENT_EVENT, transactionReceipt);
        ArrayList<AddPremiumItemEventEventResponse> responses = new ArrayList<AddPremiumItemEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            AddPremiumItemEventEventResponse typedResponse = new AddPremiumItemEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<AddPremiumItemEventEventResponse> addPremiumItemEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, AddPremiumItemEventEventResponse>() {
            @Override
            public AddPremiumItemEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(ADDPREMIUMITEMEVENT_EVENT, log);
                AddPremiumItemEventEventResponse typedResponse = new AddPremiumItemEventEventResponse();
                typedResponse.log = log;
                typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<AddPremiumItemEventEventResponse> addPremiumItemEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(ADDPREMIUMITEMEVENT_EVENT));
        return addPremiumItemEventEventFlowable(filter);
    }

    public List<EditPremiumItemEventEventResponse> getEditPremiumItemEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(EDITPREMIUMITEMEVENT_EVENT, transactionReceipt);
        ArrayList<EditPremiumItemEventEventResponse> responses = new ArrayList<EditPremiumItemEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            EditPremiumItemEventEventResponse typedResponse = new EditPremiumItemEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<EditPremiumItemEventEventResponse> editPremiumItemEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, EditPremiumItemEventEventResponse>() {
            @Override
            public EditPremiumItemEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(EDITPREMIUMITEMEVENT_EVENT, log);
                EditPremiumItemEventEventResponse typedResponse = new EditPremiumItemEventEventResponse();
                typedResponse.log = log;
                typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<EditPremiumItemEventEventResponse> editPremiumItemEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(EDITPREMIUMITEMEVENT_EVENT));
        return editPremiumItemEventEventFlowable(filter);
    }

    public List<DeletePremiumItemEventEventResponse> getDeletePremiumItemEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(DELETEPREMIUMITEMEVENT_EVENT, transactionReceipt);
        ArrayList<DeletePremiumItemEventEventResponse> responses = new ArrayList<DeletePremiumItemEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            DeletePremiumItemEventEventResponse typedResponse = new DeletePremiumItemEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<DeletePremiumItemEventEventResponse> deletePremiumItemEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, DeletePremiumItemEventEventResponse>() {
            @Override
            public DeletePremiumItemEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(DELETEPREMIUMITEMEVENT_EVENT, log);
                DeletePremiumItemEventEventResponse typedResponse = new DeletePremiumItemEventEventResponse();
                typedResponse.log = log;
                typedResponse._itemId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<DeletePremiumItemEventEventResponse> deletePremiumItemEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(DELETEPREMIUMITEMEVENT_EVENT));
        return deletePremiumItemEventEventFlowable(filter);
    }

    public List<OwnershipTransferredEventResponse> getOwnershipTransferredEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(OWNERSHIPTRANSFERRED_EVENT, transactionReceipt);
        ArrayList<OwnershipTransferredEventResponse> responses = new ArrayList<OwnershipTransferredEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            OwnershipTransferredEventResponse typedResponse = new OwnershipTransferredEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.previousOwner = (String) eventValues.getIndexedValues().get(0).getValue();
            typedResponse.newOwner = (String) eventValues.getIndexedValues().get(1).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<OwnershipTransferredEventResponse> ownershipTransferredEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, OwnershipTransferredEventResponse>() {
            @Override
            public OwnershipTransferredEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(OWNERSHIPTRANSFERRED_EVENT, log);
                OwnershipTransferredEventResponse typedResponse = new OwnershipTransferredEventResponse();
                typedResponse.log = log;
                typedResponse.previousOwner = (String) eventValues.getIndexedValues().get(0).getValue();
                typedResponse.newOwner = (String) eventValues.getIndexedValues().get(1).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<OwnershipTransferredEventResponse> ownershipTransferredEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(OWNERSHIPTRANSFERRED_EVENT));
        return ownershipTransferredEventFlowable(filter);
    }

    public List<TransferEventResponse> getTransferEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(TRANSFER_EVENT, transactionReceipt);
        ArrayList<TransferEventResponse> responses = new ArrayList<TransferEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            TransferEventResponse typedResponse = new TransferEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.from = (String) eventValues.getIndexedValues().get(0).getValue();
            typedResponse.to = (String) eventValues.getIndexedValues().get(1).getValue();
            typedResponse.value = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<TransferEventResponse> transferEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, TransferEventResponse>() {
            @Override
            public TransferEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(TRANSFER_EVENT, log);
                TransferEventResponse typedResponse = new TransferEventResponse();
                typedResponse.log = log;
                typedResponse.from = (String) eventValues.getIndexedValues().get(0).getValue();
                typedResponse.to = (String) eventValues.getIndexedValues().get(1).getValue();
                typedResponse.value = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<TransferEventResponse> transferEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(TRANSFER_EVENT));
        return transferEventFlowable(filter);
    }

    public List<ApprovalEventResponse> getApprovalEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(APPROVAL_EVENT, transactionReceipt);
        ArrayList<ApprovalEventResponse> responses = new ArrayList<ApprovalEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            ApprovalEventResponse typedResponse = new ApprovalEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.owner = (String) eventValues.getIndexedValues().get(0).getValue();
            typedResponse.spender = (String) eventValues.getIndexedValues().get(1).getValue();
            typedResponse.value = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<ApprovalEventResponse> approvalEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, ApprovalEventResponse>() {
            @Override
            public ApprovalEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(APPROVAL_EVENT, log);
                ApprovalEventResponse typedResponse = new ApprovalEventResponse();
                typedResponse.log = log;
                typedResponse.owner = (String) eventValues.getIndexedValues().get(0).getValue();
                typedResponse.spender = (String) eventValues.getIndexedValues().get(1).getValue();
                typedResponse.value = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<ApprovalEventResponse> approvalEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(APPROVAL_EVENT));
        return approvalEventFlowable(filter);
    }

    @Deprecated
    public static PremiumItemToken load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new PremiumItemToken(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static PremiumItemToken load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new PremiumItemToken(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static PremiumItemToken load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new PremiumItemToken(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static PremiumItemToken load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new PremiumItemToken(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<PremiumItemToken> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(PremiumItemToken.class, web3j, credentials, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<PremiumItemToken> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(PremiumItemToken.class, web3j, credentials, gasPrice, gasLimit, BINARY, "");
    }

    public static RemoteCall<PremiumItemToken> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(PremiumItemToken.class, web3j, transactionManager, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<PremiumItemToken> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(PremiumItemToken.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, "");
    }

    public static class AddPremiumItemEventEventResponse extends BaseEventResponse {
        public BigInteger _itemId;
    }

    public static class EditPremiumItemEventEventResponse extends BaseEventResponse {
        public BigInteger _itemId;
    }

    public static class DeletePremiumItemEventEventResponse extends BaseEventResponse {
        public BigInteger _itemId;
    }

    public static class OwnershipTransferredEventResponse extends BaseEventResponse {
        public String previousOwner;

        public String newOwner;
    }

    public static class TransferEventResponse extends BaseEventResponse {
        public String from;

        public String to;

        public BigInteger value;
    }

    public static class ApprovalEventResponse extends BaseEventResponse {
        public String owner;

        public String spender;

        public BigInteger value;
    }
}
