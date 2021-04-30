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
import org.web3j.abi.datatypes.Utf8String;
import org.web3j.abi.datatypes.generated.Bytes32;
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
public class PremiumSubscriptionToken extends Contract {
    public static final String BINARY = "6080604052600061001461006760201b60201c565b600380546001600160a01b0319166001600160a01b038316908117909155604051919250906000907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0908290a35061006b565b3390565b611bd48061007a6000396000f3fe608060405234801561001057600080fd5b50600436106101c45760003560e01c80638da5cb5b116100f9578063acabdf7511610097578063e2cee85e11610071578063e2cee85e14610399578063e9395679146103ac578063f2a90b4b146103bf578063f2fde38b146103d2576101c4565b8063acabdf7514610360578063cd05553114610373578063dd62ed3e14610386576101c4565b806394e8767d116100d357806394e8767d14610314578063a457c2d714610327578063a4faf09f1461033a578063a9059cbb1461034d576101c4565b80638da5cb5b146102e45780638f32d59b146102ec5780639201de55146102f4576101c4565b8063395093511161016657806366951e391161014057806366951e39146102a357806370a08231146102b6578063715018a6146102c957806379cc6790146102d1576101c4565b8063395093511461026857806342966c681461027b57806348b681401461028e576101c4565b80631c1c641d116101a25780631c1c641d1461021c57806323b872dd1461022f57806327de2f8d1461024257806331ecad6614610255576101c4565b8063095ea7b3146101c95780631420ee22146101f257806318160ddd14610207575b600080fd5b6101dc6101d73660046114c9565b6103e5565b6040516101e99190611997565b60405180910390f35b61020561020036600461152b565b610403565b005b61020f6104c6565b6040516101e991906119a5565b61020f61022a3660046115dd565b6104cc565b6101dc61023d36600461147c565b6104e1565b6101dc6102503660046114c9565b61056e565b6102056102633660046115dd565b6105f6565b6101dc6102763660046114c9565b61066f565b6102056102893660046115dd565b6106c3565b6102966106d7565b6040516101e99190611989565b6102056102b136600461156d565b610710565b61020f6102c4366004611424565b610822565b61020561083d565b6102056102df3660046114c9565b6108ae565b6102966108bc565b6101dc6108cb565b6103076103023660046115dd565b6108f1565b6040516101e991906119b3565b61020f6103223660046115dd565b6109f6565b6101dc6103353660046114c9565b610a43565b6102056103483660046115fb565b610ab1565b6101dc61035b3660046114c9565b610b4b565b61020561036e36600461156d565b610b5f565b6102056103813660046114f9565b610c6a565b61020f610394366004611442565b610d59565b6102056103a73660046115fb565b610d84565b6103076103ba3660046115dd565b610ea6565b6102056103cd3660046115fb565b610eb4565b6102056103e0366004611424565b610f41565b60006103f96103f2610f71565b8484610f75565b5060015b92915050565b61040b6108cb565b61043357604051600160e51b62461bcd02815260040161042a90611a14565b60405180910390fd5b60005b818110156104c1576005600084848481811061044e57fe5b602090810292909201358352508101919091526040016000908120818155600101557fa51c5188e1948c771c5869a9d2c919518028dc8e3c947dd22fd3ae13aeda4c4a83838381811061049d57fe5b905060200201356040516104b191906119a5565b60405180910390a1600101610436565b505050565b60025490565b60009081526005602052604090206001015490565b60006104ee84848461102f565b610564846104fa610f71565b61055f85604051806060016040528060288152602001611b2a602891396001600160a01b038a16600090815260016020526040812090610538610f71565b6001600160a01b03168152602081019190915260400160002054919063ffffffff61114b16565b610f75565b5060019392505050565b600060608361057c84610ea6565b60405160200161058d929190611961565b60405160208183030381529060405290506004816040516105ae919061197d565b9081526040519081900360200190205442108015906105ee57506004816040516105d8919061197d565b9081526020016040518091039020600101544211155b949350505050565b6105fe6108cb565b61061d57604051600160e51b62461bcd02815260040161042a90611a14565b60008181526005602052604080822082815560010191909155517fa51c5188e1948c771c5869a9d2c919518028dc8e3c947dd22fd3ae13aeda4c4a906106649083906119a5565b60405180910390a150565b60006103f961067c610f71565b8461055f856001600061068d610f71565b6001600160a01b03908116825260208083019390935260409182016000908120918c16815292529020549063ffffffff61117a16565b6106d46106ce610f71565b826111a9565b50565b60006106e16108cb565b61070057604051600160e51b62461bcd02815260040161042a90611a14565b506003546001600160a01b031690565b6107186108cb565b61073757604051600160e51b62461bcd02815260040161042a90611a14565b82811461074357600080fd5b60005b8381101561081b576107566113a9565b604051806040016040528087878581811061076d57fe5b90506020020135815260200185858581811061078557fe5b90506020020135815250905080600560008888868181106107a257fe5b6020908102929092013583525081810192909252604001600020825181559101516001909101557f2bf5da1e90f7b3234642edd1283811b71e4130202f7f9024b5b260ae536af5df8686848181106107f657fe5b9050602002013560405161080a91906119a5565b60405180910390a150600101610746565b5050505050565b6001600160a01b031660009081526020819052604090205490565b6108456108cb565b61086457604051600160e51b62461bcd02815260040161042a90611a14565b6003546040516000916001600160a01b0316907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0908390a3600380546001600160a01b0319169055565b6108b8828261128e565b5050565b6003546001600160a01b031690565b6003546000906001600160a01b03166108e2610f71565b6001600160a01b031614905090565b6040805160208082528183019092526060918291906020820181803883390190505090506000805b602081101561096f576008810260020a85026001600160f81b0319811615610966578084848151811061094857fe5b60200101906001600160f81b031916908160001a9053506001909201915b50600101610919565b506060816040519080825280601f01601f19166020018201604052801561099d576020820181803883390190505b50905060005b828110156109ed578381815181106109b757fe5b602001015160f81c60f81b8282815181106109ce57fe5b60200101906001600160f81b031916908160001a9053506001016109a3565b50949350505050565b600081610a0b5750600160fc1b600302610a3e565b8115610a3e5761010081049050600a8206603001600160f81b0260001b81179050600a8281610a3657fe5b049150610a0b565b919050565b60006103f9610a50610f71565b8461055f85604051806060016040528060258152602001611b766025913960016000610a7a610f71565b6001600160a01b03908116825260208083019390935260409182016000908120918d1681529252902054919063ffffffff61114b16565b610ab96108cb565b610ad857604051600160e51b62461bcd02815260040161042a90611a14565b610ae06113a9565b506040805180820182528381526020808201848152600086815260059092529083902082518155905160019091015590517f2bf5da1e90f7b3234642edd1283811b71e4130202f7f9024b5b260ae536af5df90610b3e9085906119a5565b60405180910390a1505050565b60006103f9610b58610f71565b848461102f565b610b676108cb565b610b8657604051600160e51b62461bcd02815260040161042a90611a14565b828114610b9257600080fd5b60005b8381101561081b57610ba56113a9565b6040518060400160405280878785818110610bbc57fe5b905060200201358152602001858585818110610bd457fe5b9050602002013581525090508060056000888886818110610bf157fe5b6020908102929092013583525081810192909252604001600020825181559101516001909101557f99e5fc2422da5e949a914149894c1b8cb972a4edec220e66c58a5968a668dc0d868684818110610c4557fe5b90506020020135604051610c5991906119a5565b60405180910390a150600101610b95565b610c726108cb565b610c9157604051600160e51b62461bcd02815260040161042a90611a14565b600081815260056020526040902054610ca957600080fd5b610cb3838261056e565b15610cd357604051600160e51b62461bcd02815260040161042a90611a04565b610cdb6113a9565b506040805180820190915242808252620151808402016020820152606084610d0284610ea6565b604051602001610d13929190611961565b604051602081830303815290604052905081600482604051610d35919061197d565b90815260405160209181900382019020825181559101516001909101555050505050565b6001600160a01b03918216600090815260016020908152604080832093909416825291909152205490565b600081815260056020526040902054610d9c57600080fd5b6000818152600560205260409020600101548202610db933610822565b1015610dda57604051600160e51b62461bcd02815260040161042a90611a44565b610de4338261056e565b15610e0457604051600160e51b62461bcd02815260040161042a90611a04565b600081815260056020526040902060010154610e219083026106c3565b610e296113a9565b506040805180820190915242808252620151808402016020820152606033610e5084610ea6565b604051602001610e61929190611961565b604051602081830303815290604052905081600482604051610e83919061197d565b908152604051602091819003820190208251815591015160019091015550505050565b60606103fd610302836109f6565b610ebc6108cb565b610edb57604051600160e51b62461bcd02815260040161042a90611a14565b610ee36113a9565b506040805180820182528381526020808201848152600086815260059092529083902082518155905160019091015590517f99e5fc2422da5e949a914149894c1b8cb972a4edec220e66c58a5968a668dc0d90610b3e9085906119a5565b610f496108cb565b610f6857604051600160e51b62461bcd02815260040161042a90611a14565b6106d4816112e2565b3390565b6001600160a01b038316610f9e57604051600160e51b62461bcd02815260040161042a90611a54565b6001600160a01b038216610fc757604051600160e51b62461bcd02815260040161042a906119e4565b6001600160a01b0380841660008181526001602090815260408083209487168084529490915290819020849055517f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925906110229085906119a5565b60405180910390a3505050565b6001600160a01b03831661105857604051600160e51b62461bcd02815260040161042a90611a34565b6001600160a01b03821661108157604051600160e51b62461bcd02815260040161042a906119c4565b6110c481604051806060016040528060268152602001611b04602691396001600160a01b038616600090815260208190526040902054919063ffffffff61114b16565b6001600160a01b0380851660009081526020819052604080822093909355908416815220546110f9908263ffffffff61117a16565b6001600160a01b0380841660008181526020819052604090819020939093559151908516907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef906110229085906119a5565b6000818484111561117257604051600160e51b62461bcd02815260040161042a91906119b3565b505050900390565b6000828201838110156111a257604051600160e51b62461bcd02815260040161042a906119f4565b9392505050565b6001600160a01b0382166111d257604051600160e51b62461bcd02815260040161042a90611a24565b61121581604051806060016040528060228152602001611ae2602291396001600160a01b038516600090815260208190526040902054919063ffffffff61114b16565b6001600160a01b038316600090815260208190526040902055600254611241908263ffffffff61136716565b6002556040516000906001600160a01b038416907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef906112829085906119a5565b60405180910390a35050565b61129882826111a9565b6108b8826112a4610f71565b61055f84604051806060016040528060248152602001611b52602491396001600160a01b038816600090815260016020526040812090610538610f71565b6001600160a01b03811661130b57604051600160e51b62461bcd02815260040161042a906119d4565b6003546040516001600160a01b038084169216907f8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e090600090a3600380546001600160a01b0319166001600160a01b0392909216919091179055565b60006111a283836040518060400160405280601e81526020017f536166654d6174683a207375627472616374696f6e206f766572666c6f77000081525061114b565b604051806040016040528060008152602001600081525090565b60006111a28235611a71565b60008083601f8401126113e157600080fd5b50813567ffffffffffffffff8111156113f957600080fd5b60208301915083602082028301111561141157600080fd5b9250929050565b60006111a28235611a81565b60006020828403121561143657600080fd5b60006105ee84846113c3565b6000806040838503121561145557600080fd5b600061146185856113c3565b9250506020611472858286016113c3565b9150509250929050565b60008060006060848603121561149157600080fd5b600061149d86866113c3565b93505060206114ae868287016113c3565b92505060406114bf86828701611418565b9150509250925092565b600080604083850312156114dc57600080fd5b60006114e885856113c3565b925050602061147285828601611418565b60008060006060848603121561150e57600080fd5b600061151a86866113c3565b93505060206114ae86828701611418565b6000806020838503121561153e57600080fd5b823567ffffffffffffffff81111561155557600080fd5b611561858286016113cf565b92509250509250929050565b6000806000806040858703121561158357600080fd5b843567ffffffffffffffff81111561159a57600080fd5b6115a6878288016113cf565b9450945050602085013567ffffffffffffffff8111156115c557600080fd5b6115d1878288016113cf565b95989497509550505050565b6000602082840312156115ef57600080fd5b60006105ee8484611418565b6000806040838503121561160e57600080fd5b60006114e88585611418565b61162381611a71565b82525050565b61162361163582611a71565b611ac0565b61162381611a7c565b61162381611a81565b600061165782611a64565b6116618185611a68565b9350611671818560208601611a90565b61167a81611ad1565b9093019392505050565b600061168f82611a64565b6116998185610a3e565b93506116a9818560208601611a90565b9290920192915050565b60006116c0602383611a68565b7f45524332303a207472616e7366657220746f20746865207a65726f20616464728152600160e81b6265737302602082015260400192915050565b6000611708602683611a68565b7f4f776e61626c653a206e6577206f776e657220697320746865207a65726f20618152600160d01b6564647265737302602082015260400192915050565b6000611753602283611a68565b7f45524332303a20617070726f766520746f20746865207a65726f2061646472658152600160f01b61737302602082015260400192915050565b600061179a601b83611a68565b7f536166654d6174683a206164646974696f6e206f766572666c6f770000000000815260200192915050565b60006117d3602b83611a68565b7f4572726f723a207072656d69756d20737562736372697074696f6e20697320738152600160a81b6a74696c6c2061637469766502602082015260400192915050565b6000611823602083611a68565b7f4f776e61626c653a2063616c6c6572206973206e6f7420746865206f776e6572815260200192915050565b600061185c602183611a68565b7f45524332303a206275726e2066726f6d20746865207a65726f206164647265738152600160f81b607302602082015260400192915050565b60006118a2602583611a68565b7f45524332303a207472616e736665722066726f6d20746865207a65726f2061648152600160d81b64647265737302602082015260400192915050565b60006118ec601983611a68565b7f4572726f723a20696e73756666696369656e742066756e647300000000000000815260200192915050565b6000611925602483611a68565b7f45524332303a20617070726f76652066726f6d20746865207a65726f206164648152600160e01b637265737302602082015260400192915050565b600061196d8285611629565b6014820191506105ee8284611684565b60006111a28284611684565b602081016103fd828461161a565b602081016103fd828461163a565b602081016103fd8284611643565b602080825281016111a2818461164c565b602080825281016103fd816116b3565b602080825281016103fd816116fb565b602080825281016103fd81611746565b602080825281016103fd8161178d565b602080825281016103fd816117c6565b602080825281016103fd81611816565b602080825281016103fd8161184f565b602080825281016103fd81611895565b602080825281016103fd816118df565b602080825281016103fd81611918565b5190565b90815260200190565b60006103fd82611a84565b151590565b90565b6001600160a01b031690565b60005b83811015611aab578181015183820152602001611a93565b83811115611aba576000848401525b50505050565b60006103fd8260006103fd82611adb565b601f01601f191690565b60601b9056fe45524332303a206275726e20616d6f756e7420657863656564732062616c616e636545524332303a207472616e7366657220616d6f756e7420657863656564732062616c616e636545524332303a207472616e7366657220616d6f756e74206578636565647320616c6c6f77616e636545524332303a206275726e20616d6f756e74206578636565647320616c6c6f77616e636545524332303a2064656372656173656420616c6c6f77616e63652062656c6f77207a65726fa265627a7a723058202e2787387cfd1e38fcc97aaa558c72537ac16a992cf6b858c24b24bfefff8a346c6578706572696d656e74616cf50037";

    public static final String FUNC_APPROVE = "approve";

    public static final String FUNC_BATCHDELETEPREMIUMSUBSCRIPTIONTYPES = "batchDeletePremiumSubscriptionTypes";

    public static final String FUNC_TOTALSUPPLY = "totalSupply";

    public static final String FUNC_GETPREMIUMSUBSCRIPTIONPRICEPERDAY = "getPremiumSubscriptionPricePerDay";

    public static final String FUNC_TRANSFERFROM = "transferFrom";

    public static final String FUNC_CHECKPREMIUMSUBSCRIPTION = "checkPremiumSubscription";

    public static final String FUNC_DELETEPREMIUMSUBSCRIPTIONTYPE = "deletePremiumSubscriptionType";

    public static final String FUNC_INCREASEALLOWANCE = "increaseAllowance";

    public static final String FUNC_BURN = "burn";

    public static final String FUNC_OWNERPAYABLE = "ownerPayable";

    public static final String FUNC_BATCHADDPREMIUMSUBSCRIPTIONTYPES = "batchAddPremiumSubscriptionTypes";

    public static final String FUNC_BALANCEOF = "balanceOf";

    public static final String FUNC_RENOUNCEOWNERSHIP = "renounceOwnership";

    public static final String FUNC_BURNFROM = "burnFrom";

    public static final String FUNC_OWNER = "owner";

    public static final String FUNC_ISOWNER = "isOwner";

    public static final String FUNC_BYTES32TOSTRING = "bytes32ToString";

    public static final String FUNC_UINTTOBYTES = "uintToBytes";

    public static final String FUNC_DECREASEALLOWANCE = "decreaseAllowance";

    public static final String FUNC_ADDPREMIUMSUBSCRIPTIONTYPE = "addPremiumSubscriptionType";

    public static final String FUNC_TRANSFER = "transfer";

    public static final String FUNC_BATCHEDITPREMIUMSUBSCRIPTIONTYPES = "batchEditPremiumSubscriptionTypes";

    public static final String FUNC_GRANTPREMIUMSUBSCRIPTION = "grantPremiumSubscription";

    public static final String FUNC_ALLOWANCE = "allowance";

    public static final String FUNC_BUYPREMIUMSUBSCRIPTION = "buyPremiumSubscription";

    public static final String FUNC_UINTTOSTRING = "uintToString";

    public static final String FUNC_EDITPREMIUMSUBSCRIPTIONTYPE = "editPremiumSubscriptionType";

    public static final String FUNC_TRANSFEROWNERSHIP = "transferOwnership";

    public static final Event ADDPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT = new Event("AddPremiumSubscriptionTypeEvent", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
    ;

    public static final Event EDITPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT = new Event("EditPremiumSubscriptionTypeEvent", 
            Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
    ;

    public static final Event DELETEPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT = new Event("DeletePremiumSubscriptionTypeEvent", 
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
    protected PremiumSubscriptionToken(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected PremiumSubscriptionToken(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected PremiumSubscriptionToken(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected PremiumSubscriptionToken(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
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

    public RemoteFunctionCall<TransactionReceipt> batchDeletePremiumSubscriptionTypes(List<BigInteger> _premiumSubscriptionTypeIds) {
        final Function function = new Function(
                FUNC_BATCHDELETEPREMIUMSUBSCRIPTIONTYPES, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_premiumSubscriptionTypeIds, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<BigInteger> totalSupply() {
        final Function function = new Function(FUNC_TOTALSUPPLY, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<BigInteger> getPremiumSubscriptionPricePerDay(BigInteger _premiumSubscriptionTypeId) {
        final Function function = new Function(FUNC_GETPREMIUMSUBSCRIPTIONPRICEPERDAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId)), 
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

    public RemoteFunctionCall<Boolean> checkPremiumSubscription(String _user, BigInteger _premiumSubscriptionTypeId) {
        final Function function = new Function(FUNC_CHECKPREMIUMSUBSCRIPTION, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, _user), 
                new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Bool>() {}));
        return executeRemoteCallSingleValueReturn(function, Boolean.class);
    }

    public RemoteFunctionCall<TransactionReceipt> deletePremiumSubscriptionType(BigInteger _premiumSubscriptionTypeId) {
        final Function function = new Function(
                FUNC_DELETEPREMIUMSUBSCRIPTIONTYPE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId)), 
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

    public RemoteFunctionCall<TransactionReceipt> batchAddPremiumSubscriptionTypes(List<BigInteger> _premiumSubscriptionTypeIds, List<BigInteger> _premiumSubscriptionPricesPerDay) {
        final Function function = new Function(
                FUNC_BATCHADDPREMIUMSUBSCRIPTIONTYPES, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_premiumSubscriptionTypeIds, org.web3j.abi.datatypes.generated.Uint256.class)), 
                new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_premiumSubscriptionPricesPerDay, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
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

    public RemoteFunctionCall<TransactionReceipt> burnFrom(String account, BigInteger amount) {
        final Function function = new Function(
                FUNC_BURNFROM, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, account), 
                new org.web3j.abi.datatypes.generated.Uint256(amount)), 
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

    public RemoteFunctionCall<TransactionReceipt> decreaseAllowance(String spender, BigInteger subtractedValue) {
        final Function function = new Function(
                FUNC_DECREASEALLOWANCE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, spender), 
                new org.web3j.abi.datatypes.generated.Uint256(subtractedValue)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> addPremiumSubscriptionType(BigInteger _premiumSubscriptionTypeId, BigInteger _premiumSubscriptionPricePerDay) {
        final Function function = new Function(
                FUNC_ADDPREMIUMSUBSCRIPTIONTYPE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId), 
                new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionPricePerDay)), 
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

    public RemoteFunctionCall<TransactionReceipt> batchEditPremiumSubscriptionTypes(List<BigInteger> _premiumSubscriptionTypeIds, List<BigInteger> _premiumSubscriptionPricesPerDay) {
        final Function function = new Function(
                FUNC_BATCHEDITPREMIUMSUBSCRIPTIONTYPES, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_premiumSubscriptionTypeIds, org.web3j.abi.datatypes.generated.Uint256.class)), 
                new org.web3j.abi.datatypes.DynamicArray<org.web3j.abi.datatypes.generated.Uint256>(
                        org.web3j.abi.datatypes.generated.Uint256.class,
                        org.web3j.abi.Utils.typeMap(_premiumSubscriptionPricesPerDay, org.web3j.abi.datatypes.generated.Uint256.class))), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> grantPremiumSubscription(String _user, BigInteger _days, BigInteger _premiumSubscriptionTypeId) {
        final Function function = new Function(
                FUNC_GRANTPREMIUMSUBSCRIPTION, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, _user), 
                new org.web3j.abi.datatypes.generated.Uint256(_days), 
                new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<BigInteger> allowance(String owner, String spender) {
        final Function function = new Function(FUNC_ALLOWANCE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, owner), 
                new org.web3j.abi.datatypes.Address(160, spender)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> buyPremiumSubscription(BigInteger _days, BigInteger _premiumSubscriptionTypeId) {
        final Function function = new Function(
                FUNC_BUYPREMIUMSUBSCRIPTION, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_days), 
                new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> uintToString(BigInteger v) {
        final Function function = new Function(FUNC_UINTTOSTRING, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(v)), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<TransactionReceipt> editPremiumSubscriptionType(BigInteger _premiumSubscriptionTypeId, BigInteger _premiumSubscriptionPricePerDay) {
        final Function function = new Function(
                FUNC_EDITPREMIUMSUBSCRIPTIONTYPE, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionTypeId), 
                new org.web3j.abi.datatypes.generated.Uint256(_premiumSubscriptionPricePerDay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> transferOwnership(String newOwner) {
        final Function function = new Function(
                FUNC_TRANSFEROWNERSHIP, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Address(160, newOwner)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public List<AddPremiumSubscriptionTypeEventEventResponse> getAddPremiumSubscriptionTypeEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(ADDPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, transactionReceipt);
        ArrayList<AddPremiumSubscriptionTypeEventEventResponse> responses = new ArrayList<AddPremiumSubscriptionTypeEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            AddPremiumSubscriptionTypeEventEventResponse typedResponse = new AddPremiumSubscriptionTypeEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<AddPremiumSubscriptionTypeEventEventResponse> addPremiumSubscriptionTypeEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, AddPremiumSubscriptionTypeEventEventResponse>() {
            @Override
            public AddPremiumSubscriptionTypeEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(ADDPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, log);
                AddPremiumSubscriptionTypeEventEventResponse typedResponse = new AddPremiumSubscriptionTypeEventEventResponse();
                typedResponse.log = log;
                typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<AddPremiumSubscriptionTypeEventEventResponse> addPremiumSubscriptionTypeEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(ADDPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT));
        return addPremiumSubscriptionTypeEventEventFlowable(filter);
    }

    public List<EditPremiumSubscriptionTypeEventEventResponse> getEditPremiumSubscriptionTypeEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(EDITPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, transactionReceipt);
        ArrayList<EditPremiumSubscriptionTypeEventEventResponse> responses = new ArrayList<EditPremiumSubscriptionTypeEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            EditPremiumSubscriptionTypeEventEventResponse typedResponse = new EditPremiumSubscriptionTypeEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<EditPremiumSubscriptionTypeEventEventResponse> editPremiumSubscriptionTypeEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, EditPremiumSubscriptionTypeEventEventResponse>() {
            @Override
            public EditPremiumSubscriptionTypeEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(EDITPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, log);
                EditPremiumSubscriptionTypeEventEventResponse typedResponse = new EditPremiumSubscriptionTypeEventEventResponse();
                typedResponse.log = log;
                typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<EditPremiumSubscriptionTypeEventEventResponse> editPremiumSubscriptionTypeEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(EDITPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT));
        return editPremiumSubscriptionTypeEventEventFlowable(filter);
    }

    public List<DeletePremiumSubscriptionTypeEventEventResponse> getDeletePremiumSubscriptionTypeEventEvents(TransactionReceipt transactionReceipt) {
        List<Contract.EventValuesWithLog> valueList = extractEventParametersWithLog(DELETEPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, transactionReceipt);
        ArrayList<DeletePremiumSubscriptionTypeEventEventResponse> responses = new ArrayList<DeletePremiumSubscriptionTypeEventEventResponse>(valueList.size());
        for (Contract.EventValuesWithLog eventValues : valueList) {
            DeletePremiumSubscriptionTypeEventEventResponse typedResponse = new DeletePremiumSubscriptionTypeEventEventResponse();
            typedResponse.log = eventValues.getLog();
            typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
            responses.add(typedResponse);
        }
        return responses;
    }

    public Flowable<DeletePremiumSubscriptionTypeEventEventResponse> deletePremiumSubscriptionTypeEventEventFlowable(EthFilter filter) {
        return web3j.ethLogFlowable(filter).map(new io.reactivex.functions.Function<Log, DeletePremiumSubscriptionTypeEventEventResponse>() {
            @Override
            public DeletePremiumSubscriptionTypeEventEventResponse apply(Log log) {
                Contract.EventValuesWithLog eventValues = extractEventParametersWithLog(DELETEPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT, log);
                DeletePremiumSubscriptionTypeEventEventResponse typedResponse = new DeletePremiumSubscriptionTypeEventEventResponse();
                typedResponse.log = log;
                typedResponse._premiumSubscriptionTypeId = (BigInteger) eventValues.getNonIndexedValues().get(0).getValue();
                return typedResponse;
            }
        });
    }

    public Flowable<DeletePremiumSubscriptionTypeEventEventResponse> deletePremiumSubscriptionTypeEventEventFlowable(DefaultBlockParameter startBlock, DefaultBlockParameter endBlock) {
        EthFilter filter = new EthFilter(startBlock, endBlock, getContractAddress());
        filter.addSingleTopic(EventEncoder.encode(DELETEPREMIUMSUBSCRIPTIONTYPEEVENT_EVENT));
        return deletePremiumSubscriptionTypeEventEventFlowable(filter);
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
    public static PremiumSubscriptionToken load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new PremiumSubscriptionToken(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static PremiumSubscriptionToken load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new PremiumSubscriptionToken(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static PremiumSubscriptionToken load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new PremiumSubscriptionToken(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static PremiumSubscriptionToken load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new PremiumSubscriptionToken(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<PremiumSubscriptionToken> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(PremiumSubscriptionToken.class, web3j, credentials, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<PremiumSubscriptionToken> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(PremiumSubscriptionToken.class, web3j, credentials, gasPrice, gasLimit, BINARY, "");
    }

    public static RemoteCall<PremiumSubscriptionToken> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return deployRemoteCall(PremiumSubscriptionToken.class, web3j, transactionManager, contractGasProvider, BINARY, "");
    }

    @Deprecated
    public static RemoteCall<PremiumSubscriptionToken> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return deployRemoteCall(PremiumSubscriptionToken.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, "");
    }

    public static class AddPremiumSubscriptionTypeEventEventResponse extends BaseEventResponse {
        public BigInteger _premiumSubscriptionTypeId;
    }

    public static class EditPremiumSubscriptionTypeEventEventResponse extends BaseEventResponse {
        public BigInteger _premiumSubscriptionTypeId;
    }

    public static class DeletePremiumSubscriptionTypeEventEventResponse extends BaseEventResponse {
        public BigInteger _premiumSubscriptionTypeId;
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
