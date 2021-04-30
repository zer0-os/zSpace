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
public class ERC721Mintable extends Contract {
    public static final String BINARY = "60806040526200001c6301ffc9a760e01b6200005a60201b60201c565b620000346380ac58cd60e01b6200005a60201b60201c565b62000054620000486200012960201b60201c565b6200012d60201b60201c565b620002a6565b7fffffffff000000000000000000000000000000000000000000000000000000008082161415620000ec57604080517f08c379a000000000000000000000000000000000000000000000000000000000815260206004820152601c60248201527f4552433136353a20696e76616c696420696e7465726661636520696400000000604482015290519081900360640190fd5b7fffffffff00000000000000000000000000000000000000000000000000000000166000908152602081905260409020805460ff19166001179055565b3390565b620001488160056200017f60201b620012bc1790919060201c565b6040516001600160a01b038216907f6ae172837ea30b801fbfcdd4108aa1d5bf8ff775444fd70256b44e6bf3dfc3f690600090a250565b6200019182826200022360201b60201c565b15620001fe57604080517f08c379a000000000000000000000000000000000000000000000000000000000815260206004820152601f60248201527f526f6c65733a206163636f756e7420616c72656164792068617320726f6c6500604482015290519081900360640190fd5b6001600160a01b0316600090815260209190915260409020805460ff19166001179055565b60006001600160a01b03821662000286576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401808060200182810382526022815260200180620019cf6022913960400191505060405180910390fd5b506001600160a01b03166000908152602091909152604090205460ff1690565b61171980620002b66000396000f3fe608060405234801561001057600080fd5b50600436106101005760003560e01c80638832e6e311610097578063a22cb46511610066578063a22cb465146103a9578063aa271e1a146103d7578063b88d4fde146103fd578063e985e9c5146104c357610100565b80638832e6e314610294578063983b2d561461034f5780639865027514610375578063a14481941461037d57610100565b806340c10f19116100d357806340c10f19146101dd57806342842e0e146102095780636352211e1461023f57806370a082311461025c57610100565b806301ffc9a714610105578063081812fc14610140578063095ea7b31461017957806323b872dd146101a7575b600080fd5b61012c6004803603602081101561011b57600080fd5b50356001600160e01b0319166104f1565b604080519115158252519081900360200190f35b61015d6004803603602081101561015657600080fd5b5035610510565b604080516001600160a01b039092168252519081900360200190f35b6101a56004803603604081101561018f57600080fd5b506001600160a01b038135169060200135610575565b005b6101a5600480360360608110156101bd57600080fd5b506001600160a01b038135811691602081013590911690604001356106a3565b61012c600480360360408110156101f357600080fd5b506001600160a01b038135169060200135610702565b6101a56004803603606081101561021f57600080fd5b506001600160a01b03813581169160208101359091169060400135610765565b61015d6004803603602081101561025557600080fd5b5035610780565b6102826004803603602081101561027257600080fd5b50356001600160a01b03166107dd565b60408051918252519081900360200190f35b61012c600480360360608110156102aa57600080fd5b6001600160a01b03823516916020810135918101906060810160408201356401000000008111156102da57600080fd5b8201836020820111156102ec57600080fd5b8035906020019184600183028401116401000000008311171561030e57600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929550610848945050505050565b6101a56004803603602081101561036557600080fd5b50356001600160a01b03166108a8565b6101a56108fd565b61012c6004803603604081101561039357600080fd5b506001600160a01b03813516906020013561090f565b6101a5600480360360408110156103bf57600080fd5b506001600160a01b0381351690602001351515610964565b61012c600480360360208110156103ed57600080fd5b50356001600160a01b0316610a6c565b6101a56004803603608081101561041357600080fd5b6001600160a01b0382358116926020810135909116916040820135919081019060808101606082013564010000000081111561044e57600080fd5b82018360208201111561046057600080fd5b8035906020019184600183028401116401000000008311171561048257600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929550610a7f945050505050565b61012c600480360360408110156104d957600080fd5b506001600160a01b0381358116916020013516610ae0565b6001600160e01b03191660009081526020819052604090205460ff1690565b600061051b82610b0e565b61055957604051600160e51b62461bcd02815260040180806020018281038252602c815260200180611625602c913960400191505060405180910390fd5b506000908152600260205260409020546001600160a01b031690565b600061058082610780565b9050806001600160a01b0316836001600160a01b031614156105d657604051600160e51b62461bcd02815260040180806020018281038252602181526020018061169c6021913960400191505060405180910390fd5b806001600160a01b03166105e8610b2b565b6001600160a01b03161480610609575061060981610604610b2b565b610ae0565b61064757604051600160e51b62461bcd0281526004018080602001828103825260388152602001806115496038913960400191505060405180910390fd5b60008281526002602052604080822080546001600160a01b0319166001600160a01b0387811691821790925591518593918516917f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b92591a4505050565b6106b46106ae610b2b565b82610b2f565b6106f257604051600160e51b62461bcd0281526004018080602001828103825260318152602001806116bd6031913960400191505060405180910390fd5b6106fd838383610bd6565b505050565b600061071461070f610b2b565b610a6c565b61075257604051600160e51b62461bcd0281526004018080602001828103825260308152602001806115d46030913960400191505060405180910390fd5b61075c8383610d20565b50600192915050565b6106fd83838360405180602001604052806000815250610a7f565b6000818152600160205260408120546001600160a01b0316806107d757604051600160e51b62461bcd0281526004018080602001828103825260298152602001806115ab6029913960400191505060405180910390fd5b92915050565b60006001600160a01b03821661082757604051600160e51b62461bcd02815260040180806020018281038252602a815260200180611581602a913960400191505060405180910390fd5b6001600160a01b03821660009081526003602052604090206107d790610e57565b600061085561070f610b2b565b61089357604051600160e51b62461bcd0281526004018080602001828103825260308152602001806115d46030913960400191505060405180910390fd5b61089e848484610e5b565b5060019392505050565b6108b361070f610b2b565b6108f157604051600160e51b62461bcd0281526004018080602001828103825260308152602001806115d46030913960400191505060405180910390fd5b6108fa81610eb0565b50565b61090d610908610b2b565b610ef8565b565b600061091c61070f610b2b565b61095a57604051600160e51b62461bcd0281526004018080602001828103825260308152602001806115d46030913960400191505060405180910390fd5b61075c8383610f40565b61096c610b2b565b6001600160a01b0316826001600160a01b031614156109d55760408051600160e51b62461bcd02815260206004820152601960248201527f4552433732313a20617070726f766520746f2063616c6c657200000000000000604482015290519081900360640190fd5b80600460006109e2610b2b565b6001600160a01b03908116825260208083019390935260409182016000908120918716808252919093529120805460ff191692151592909217909155610a26610b2b565b60408051841515815290516001600160a01b0392909216917f17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c319181900360200190a35050565b60006107d760058363ffffffff610f5e16565b610a90610a8a610b2b565b83610b2f565b610ace57604051600160e51b62461bcd0281526004018080602001828103825260318152602001806116bd6031913960400191505060405180910390fd5b610ada84848484610fc8565b50505050565b6001600160a01b03918216600090815260046020908152604080832093909416825291909152205460ff1690565b6000908152600160205260409020546001600160a01b0316151590565b3390565b6000610b3a82610b0e565b610b7857604051600160e51b62461bcd02815260040180806020018281038252602c81526020018061151d602c913960400191505060405180910390fd5b6000610b8383610780565b9050806001600160a01b0316846001600160a01b03161480610bbe5750836001600160a01b0316610bb384610510565b6001600160a01b0316145b80610bce5750610bce8185610ae0565b949350505050565b826001600160a01b0316610be982610780565b6001600160a01b031614610c3157604051600160e51b62461bcd0281526004018080602001828103825260298152602001806116736029913960400191505060405180910390fd5b6001600160a01b038216610c7957604051600160e51b62461bcd0281526004018080602001828103825260248152602001806114f96024913960400191505060405180910390fd5b610c828161101d565b6001600160a01b0383166000908152600360205260409020610ca390611058565b6001600160a01b0382166000908152600360205260409020610cc49061106f565b60008181526001602052604080822080546001600160a01b0319166001600160a01b0386811691821790925591518493918716917fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef91a4505050565b6001600160a01b038216610d7e5760408051600160e51b62461bcd02815260206004820181905260248201527f4552433732313a206d696e7420746f20746865207a65726f2061646472657373604482015290519081900360640190fd5b610d8781610b0e565b15610ddc5760408051600160e51b62461bcd02815260206004820152601c60248201527f4552433732313a20746f6b656e20616c7265616479206d696e74656400000000604482015290519081900360640190fd5b600081815260016020908152604080832080546001600160a01b0319166001600160a01b038716908117909155835260039091529020610e1b9061106f565b60405181906001600160a01b038416906000907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef908290a45050565b5490565b610e658383610d20565b610e726000848484611078565b6106fd57604051600160e51b62461bcd0281526004018080602001828103825260328152602001806114c76032913960400191505060405180910390fd5b610ec160058263ffffffff6112bc16565b6040516001600160a01b038216907f6ae172837ea30b801fbfcdd4108aa1d5bf8ff775444fd70256b44e6bf3dfc3f690600090a250565b610f0960058263ffffffff61134016565b6040516001600160a01b038216907fe94479a9f7e1952cc78f2d6baab678adc1b772d936c6583def489e524cb6669290600090a250565b610f5a828260405180602001604052806000815250610e5b565b5050565b60006001600160a01b038216610fa857604051600160e51b62461bcd0281526004018080602001828103825260228152602001806116516022913960400191505060405180910390fd5b506001600160a01b03166000908152602091909152604090205460ff1690565b610fd3848484610bd6565b610fdf84848484611078565b610ada57604051600160e51b62461bcd0281526004018080602001828103825260328152602001806114c76032913960400191505060405180910390fd5b6000818152600260205260409020546001600160a01b0316156108fa57600090815260026020526040902080546001600160a01b0319169055565b805461106b90600163ffffffff6113aa16565b9055565b80546001019055565b600061108c846001600160a01b03166113f3565b61109857506001610bce565b600060606001600160a01b038616600160e11b630a85bd01026110b9610b2b565b89888860405160240180856001600160a01b03166001600160a01b03168152602001846001600160a01b03166001600160a01b0316815260200183815260200180602001828103825283818151815260200191508051906020019080838360005b8381101561113257818101518382015260200161111a565b50505050905090810190601f16801561115f5780820380516001836020036101000a031916815260200191505b5060408051601f198184030181529181526020820180516001600160e01b03166001600160e01b0319909a16999099178952518151919890975087965094509250829150849050835b602083106111c75780518252601f1990920191602091820191016111a8565b6001836020036101000a0380198251168184511680821785525050505050509050019150506000604051808303816000865af19150503d8060008114611229576040519150601f19603f3d011682016040523d82523d6000602084013e61122e565b606091505b509150915081611282578051156112485780518082602001fd5b604051600160e51b62461bcd0281526004018080602001828103825260328152602001806114c76032913960400191505060405180910390fd5b600081806020019051602081101561129957600080fd5b50516001600160e01b031916600160e11b630a85bd0102149350610bce92505050565b6112c68282610f5e565b1561131b5760408051600160e51b62461bcd02815260206004820152601f60248201527f526f6c65733a206163636f756e7420616c72656164792068617320726f6c6500604482015290519081900360640190fd5b6001600160a01b0316600090815260209190915260409020805460ff19166001179055565b61134a8282610f5e565b61138857604051600160e51b62461bcd0281526004018080602001828103825260218152602001806116046021913960400191505060405180910390fd5b6001600160a01b0316600090815260209190915260409020805460ff19169055565b60006113ec83836040518060400160405280601e81526020017f536166654d6174683a207375627472616374696f6e206f766572666c6f77000081525061142c565b9392505050565b6000813f7fc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470818114801590610bce575050151592915050565b600081848411156114be57604051600160e51b62461bcd0281526004018080602001828103825283818151815260200191508051906020019080838360005b8381101561148357818101518382015260200161146b565b50505050905090810190601f1680156114b05780820380516001836020036101000a031916815260200191505b509250505060405180910390fd5b50505090039056fe4552433732313a207472616e7366657220746f206e6f6e20455243373231526563656976657220696d706c656d656e7465724552433732313a207472616e7366657220746f20746865207a65726f20616464726573734552433732313a206f70657261746f7220717565727920666f72206e6f6e6578697374656e7420746f6b656e4552433732313a20617070726f76652063616c6c6572206973206e6f74206f776e6572206e6f7220617070726f76656420666f7220616c6c4552433732313a2062616c616e636520717565727920666f7220746865207a65726f20616464726573734552433732313a206f776e657220717565727920666f72206e6f6e6578697374656e7420746f6b656e4d696e746572526f6c653a2063616c6c657220646f6573206e6f74206861766520746865204d696e74657220726f6c65526f6c65733a206163636f756e7420646f6573206e6f74206861766520726f6c654552433732313a20617070726f76656420717565727920666f72206e6f6e6578697374656e7420746f6b656e526f6c65733a206163636f756e7420697320746865207a65726f20616464726573734552433732313a207472616e73666572206f6620746f6b656e2074686174206973206e6f74206f776e4552433732313a20617070726f76616c20746f2063757272656e74206f776e65724552433732313a207472616e736665722063616c6c6572206973206e6f74206f776e6572206e6f7220617070726f766564a165627a7a72305820140e5c36daeb0369f9404d2f4c66956d4a7b2870477133ec8c457a64d3a808760029526f6c65733a206163636f756e7420697320746865207a65726f2061646472657373";

    public static final String FUNC_SUPPORTSINTERFACE = "supportsInterface";

    public static final String FUNC_GETAPPROVED = "getApproved";

    public static final String FUNC_APPROVE = "approve";

    public static final String FUNC_TRANSFERFROM = "transferFrom";

    public static final String FUNC_MINT = "mint";

    public static final String FUNC_safeTransferFrom = "safeTransferFrom";

    public static final String FUNC_OWNEROF = "ownerOf";

    public static final String FUNC_BALANCEOF = "balanceOf";

    public static final String FUNC_safeMint = "safeMint";

    public static final String FUNC_ADDMINTER = "addMinter";

    public static final String FUNC_RENOUNCEMINTER = "renounceMinter";

    public static final String FUNC_SETAPPROVALFORALL = "setApprovalForAll";

    public static final String FUNC_ISMINTER = "isMinter";

    public static final String FUNC_ISAPPROVEDFORALL = "isApprovedForAll";

    public static final Event MINTERADDED_EVENT = new Event("MinterAdded", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}));
    ;

    public static final Event MINTERREMOVED_EVENT = new Event("MinterRemoved", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}));
    ;

    public static final Event TRANSFER_EVENT = new Event("Transfer", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}, new TypeReference<Uint256>(true) {}));
    ;

    public static final Event APPROVAL_EVENT = new Event("Approval", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}, new TypeReference<Uint256>(true) {}));
    ;

    public static final Event APPROVALFORALL_EVENT = new Event("ApprovalForAll", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Address>(true) {}, new TypeReference<Address>(true) {}, new TypeReference<Bool>() {}));
    ;

    @Deprecated
    protected ERC721Mintable(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected ERC721Mintable(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected ERC721Mintable(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected ERC721Mintable(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<Boolean> supportsInterface(byte[] interfaceId) {
        final Function function = new Function(FUNC_SUPPORTSINTERFACE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Bytes4(interfaceId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<String> getApproved(BigInteger tokenId) {
        final Function function = new Function(FUNC_GETAPPROVED, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Address>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<TransactionReceipt> approve(String to, BigInteger tokenId) {
        final Function function = new Function(
                FUNC_APPROVE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> transferFrom(String from, String to, BigInteger tokenId) {
        final Function function = new Function(
                FUNC_TRANSFERFROM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, from), 
                new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> mint(String to, BigInteger tokenId) {
        final Function function = new Function(
                FUNC_MINT, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> safeTransferFrom(String from, String to, BigInteger tokenId) {
        final Function function = new Function(
                FUNC_safeTransferFrom, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, from), 
                new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> ownerOf(BigInteger tokenId) {
        final Function function = new Function(FUNC_OWNEROF, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Address>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<BigInteger> balanceOf(String owner) {
        final Function function = new Function(FUNC_BALANCEOF, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, owner)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> safeMint(String to, BigInteger tokenId, byte[] _data) {
        final Function function = new Function(
                FUNC_safeMint, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId), 
                new org.web3j.abi.datatypes.DynamicBytes(_data)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> addMinter(String account) {
        final Function function = new Function(
                FUNC_ADDMINTER, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, account)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> renounceMinter() {
        final Function function = new Function(
                FUNC_RENOUNCEMINTER, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> safeMint(String to, BigInteger tokenId) {
        final Function function = new Function(
                FUNC_safeMint, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setApprovalForAll(String to, Boolean approved) {
        final Function function = new Function(
                FUNC_SETAPPROVALFORALL, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.Bool(approved)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<Boolean> isMinter(String account) {
        final Function function = new Function(FUNC_ISMINTER, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, account)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<TransactionReceipt> safeTransferFrom(String from, String to, BigInteger tokenId, byte[] _data) {
        final Function function = new Function(
                FUNC_safeTransferFrom, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, from), 
                new org.web3j.abi.datatypes.Address(160, to), 
                new org.web3j.abi.datatypes.generated.Uint256(tokenId), 
                new org.web3j.abi.datatypes.DynamicBytes(_data)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<Boolean> isApprovedForAll(String owner, String operator) {
        final Function function = new Function(FUNC_ISAPPROVEDFORALL, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, owner), 
                new org.web3j.abi.datatypes.Address(160, operator)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public List<MinterAddedEventResponse> getMinterAddedEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(MINTERADDED_EVENT, transactionReceipt);
        ArrayList<MinterAddedEventResponse> responses = new ArrayList<MinterAddedEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            MinterAddedEventResponse typedResponse = new MinterAddedEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.account = (String) eventValues.getIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<MinterAddedEventResponse> minterAddedEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, MinterAddedEventResponse>() {
            @Override
            public MinterAddedEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(MINTERADDED_EVENT, log);
                MinterAddedEventResponse typedResponse = new MinterAddedEventResponse();
                typedResponse.log = log;
                typedResponse.account = (String) eventValues.getIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<MinterAddedEventResponse> minterAddedEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(MINTERADDED_EVENT));
        return minterAddedEventFlowable(filter);
    }

    public List<MinterRemovedEventResponse> getMinterRemovedEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(MINTERREMOVED_EVENT, transactionReceipt);
        ArrayList<MinterRemovedEventResponse> responses = new ArrayList<MinterRemovedEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            MinterRemovedEventResponse typedResponse = new MinterRemovedEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.account = (String) eventValues.getIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<MinterRemovedEventResponse> minterRemovedEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, MinterRemovedEventResponse>() {
            @Override
            public MinterRemovedEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(MINTERREMOVED_EVENT, log);
                MinterRemovedEventResponse typedResponse = new MinterRemovedEventResponse();
                typedResponse.log = log;
                typedResponse.account = (String) eventValues.getIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<MinterRemovedEventResponse> minterRemovedEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(MINTERREMOVED_EVENT));
        return minterRemovedEventFlowable(filter);
    }

    public List<TransferEventResponse> getTransferEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(TRANSFER_EVENT, transactionReceipt);
        ArrayList<TransferEventResponse> responses = new ArrayList<TransferEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            TransferEventResponse typedResponse = new TransferEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.from = (String) eventValues.getIndexedValues().get(0).getValue();
            typedResponse.to = (String) eventValues.getIndexedValues().get(1).getValue();
            typedResponse.tokenId = (BigInteger) eventValues.getIndexedValues().get(2).getValue();
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
                typedResponse.tokenId = (BigInteger) eventValues.getIndexedValues().get(2).getValue();
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
            typedResponse.approved = (String) eventValues.getIndexedValues().get(1).getValue();
            typedResponse.tokenId = (BigInteger) eventValues.getIndexedValues().get(2).getValue();
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
                typedResponse.approved = (String) eventValues.getIndexedValues().get(1).getValue();
                typedResponse.tokenId = (BigInteger) eventValues.getIndexedValues().get(2).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<ApprovalEventResponse> approvalEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(APPROVAL_EVENT));
        return approvalEventFlowable(filter);
    }

    public List<ApprovalForAllEventResponse> getApprovalForAllEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(APPROVALFORALL_EVENT, transactionReceipt);
        ArrayList<ApprovalForAllEventResponse> responses = new ArrayList<ApprovalForAllEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            ApprovalForAllEventResponse typedResponse = new ApprovalForAllEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse.owner = (String) eventValues.getIndexedValues().get(0).getValue();
            typedResponse.operator = (String) eventValues.getIndexedValues().get(1).getValue();
            typedResponse.approved = (Boolean) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<ApprovalForAllEventResponse> approvalForAllEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, ApprovalForAllEventResponse>() {
            @Override
            public ApprovalForAllEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(APPROVALFORALL_EVENT, log);
                ApprovalForAllEventResponse typedResponse = new ApprovalForAllEventResponse();
                typedResponse.log = log;
                typedResponse.owner = (String) eventValues.getIndexedValues().get(0).getValue();
                typedResponse.operator = (String) eventValues.getIndexedValues().get(1).getValue();
                typedResponse.approved = (Boolean) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<ApprovalForAllEventResponse> approvalForAllEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(APPROVALFORALL_EVENT));
        return approvalForAllEventFlowable(filter);
    }

    @Deprecated
    public static ERC721Mintable load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new ERC721Mintable(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static ERC721Mintable load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new ERC721Mintable(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static ERC721Mintable load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new ERC721Mintable(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static ERC721Mintable load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new ERC721Mintable(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<ERC721Mintable> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(ERC721Mintable.class, web3j, credentials, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<ERC721Mintable> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(ERC721Mintable.class, web3j, credentials, gasPrice, gasLimit, BINARY, "");
    }

    public static RemoteCall<ERC721Mintable> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(ERC721Mintable.class, web3j, transactionManager, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<ERC721Mintable> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(ERC721Mintable.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, "");
    }

    public static class MinterAddedEventResponse extends BaseEventResponse {
        public String account;
    }

    public static class MinterRemovedEventResponse extends BaseEventResponse {
        public String account;
    }

    public static class TransferEventResponse extends BaseEventResponse {
        public String from;

        public String to;

        public BigInteger tokenId;
    }

    public static class ApprovalEventResponse extends BaseEventResponse {
        public String owner;

        public String approved;

        public BigInteger tokenId;
    }

    public static class ApprovalForAllEventResponse extends BaseEventResponse {
        public String owner;

        public String operator;

        public Boolean approved;
    }
}
