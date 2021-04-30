// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "EtherlinkerTypes.generated.h"


/**
* Wallet authentication request
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FWalletAuthenticationRequest {

	GENERATED_USTRUCT_BODY()

	/**
	 * Unique sender identifier
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString userIndex;

	/**
	 * Link to the Integration Server
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString serverAddress;

	/**
	 * User login for remote authentication
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString login;

	/**
	 * User password for remote authentication
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString password;

	/**
	 * Path to the user wallet or directory, where wallets located
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString walletPath;

	/**
	 * Version of request data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherlinkerRemoteWalletManager")
	FString requestDataVersion;

	FWalletAuthenticationRequest() {
		senderId = "";
		userIndex = "0";
		serverAddress = "https://localhost:8451";
		login = "";
		password = "";
		walletPath = "";
		requestDataVersion = "1";
	}
};

/**
 * Wallet authentication response
 */
USTRUCT(BlueprintType)
struct ETHERLINKER_API FWalletAuthenticationResponse {

	GENERATED_USTRUCT_BODY()

	/**
	 * Unique sender identifier
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString userIndex;

	/**
	 * Custom response data
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString data;

	/**
	 * Address of the user wallet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString walletAddress;

	/**
	 * Password of the user wallet
	 * Can be empty
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString walletPassword;

	/**
	 * BIP39 mnemonic of the user wallet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString walletMnemonic;

	/**
	 * Path to the user wallet or directory, where wallets located
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString walletPath;

	/**
	 * Operation type, which was used during method execution (like "getWalletData" or "createUserAccount")
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherlinkerRemoteWalletManager")
	FString operationType;

	/**
	 * Version of response data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherlinkerRemoteWalletManager")
	FString responseDataVersion;

	FWalletAuthenticationResponse() {
		senderId = "";
		data = "";
		userIndex = "0";
		responseDataVersion = "1";
		walletAddress = "";
		walletPassword = "";
		operationType = "";
		walletMnemonic = "";
		walletPath = "";
	}

};

/**
* JSON request data
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FEtherlinkerRequestData {

	GENERATED_USTRUCT_BODY()

	/**
     * Unique sender identifier
     */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString userIndex;

	/**
	 * Link to the Integration Server
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString serverAddress;

	/**
	 * Address of the user wallet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletAddress;

	/**
	 * Password of the user wallet
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString walletPassword;

	/**
	 * BIP39 mnemonic of the user wallet
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString walletMnemonic;

	/**
	 * Path to the user wallet or directory, where wallets located
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString walletPath;

	/**
	 * Authentication type.
	 * Can be "Mnemonic" or "PrivateKey"
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString walletAuthType;

	/**
	 * Address of the deployed solidity contract
	 * Example: 0x5d260ae2d5014e450a70dc24be0f191353ec1a1e
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractAddress;

	/**
	 * Method, which should be executed. 
	 * Example: stringTest
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractMethodName;

	/**
	 * Contract name, which is the name of a Java class wrapper of solidity contract, which you will get after contract compilation.
	 * Example: com.academy.contracts.MethodParamsTester
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractName;

	/**
	 * Version of request data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString requestDataVersion;

	/**
	 * Custom input parameters for the selected contract method.
	 * Should be in the same order as in solidity contract.
	 * If you have an array parameter, you should specify all its value in one parameter.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	TArray<FString> contractMethodParams;

	/**
	 * Types of custom input parameters for the selected contract method.
	 * Should be in the same order as in solidity contract.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	TArray<FString> contractMethodParamTypes;

	/**
	 * Allows you to receive results in user-friendly ETH format instead of long Wei format with a lot of digits.
	 * It's recommended to set it true in transferEther, getWalletBalance operations and in execContractMethod if the method returns value in Wei format by default.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString convertResultFromWeiToEth;

	/**
	 * The higher the value, the faster your transactions will be processed.
	 * The value should be in Wei format.
	 * Default value: 22000000000
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString gasPrice;

	/**
	 * The maximum amount of units of gas you are willing to spend on a transaction.
	 * The default value is 4300000
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString gasLimit;

	/**
	 * You should specify your custom access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString InfuraURL;

	/**
	 * Can be used with transferEther operationType to specify wallet on which you will send ETH.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString receiverAddress;

	/**
	 * Can be used with transferEther operationType in order to specify how much ETH you want to send.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString ethAmountToSend;

	/**
	 * Operation type, which was used during method execution (like "createWallet" or "deployContract")
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString operationType;

	FEtherlinkerRequestData() {
		senderId = "";
		userIndex = "0";
		serverAddress = "https://localhost:8451";
		walletAuthType = "Mnemonic";
		walletAddress = "";
		walletPassword = "";
		walletMnemonic = "";
		walletPath = "";
		contractAddress = "";
		contractMethodName = "";
		contractName = "";
		requestDataVersion = "1";
		convertResultFromWeiToEth = "false";
		gasPrice = "22000000000";
		gasLimit = "4300000";
		InfuraURL = "";
		receiverAddress = "";
		ethAmountToSend = "";
		operationType = "";
	}
};

/**
* Wallet data to enhance requests with it from single point instead of entering it to each request manually
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FWalletData {

	GENERATED_USTRUCT_BODY()

	/**
     * Address of the user wallet
     */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletAddress;

	/**
	 * Password of the user wallet
	 * Can be empty
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletPassword;

	/**
	 * BIP39 mnemonic of the user wallet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletMnemonic;

	/**
	 * Path to the user wallet or directory, where wallets located
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletPath;

	/**
	 * Authentication type.
	 * Can be "Mnemonic" or "PrivateKey"
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletAuthType;

	FWalletData() {
		walletAddress = "";
		walletPassword = "";
		walletMnemonic = "";
		walletPath = "";
		walletAuthType = "Mnemonic";
	}
};

/**
* JSON response data
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FEtherlinkerResponseData {

	GENERATED_USTRUCT_BODY()

	/**
     * Unique sender identifier
     */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString userIndex;

	/**
	 * Custom response data (can be transaction receipt, wallet balance, etc)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString data;

	/**
	 * Operation type, which was used during method execution (like "createWallet" or "deployContract")
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString operationType;

	/**
	 * Address of the user wallet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString walletAddress;

	/**
	 * Address of the deployed solidity contract
	 * Example: 0x5d260ae2d5014e450a70dc24be0f191353ec1a1e
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractAddress;

	/**
	 * Method, which should be executed. Example: stringTest
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractMethodName;

	/**
	 * Contract name, which is the name of a Java class wrapper of solidity contract, which you will get after contract compilation.
	 * Example: com.academy.contracts.MethodParamsTester
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString contractName;

	/**
	 * Version of response data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString responseDataVersion;

	FEtherlinkerResponseData() {
		senderId = "";
		data = "";
		userIndex = "0";
		operationType = "";
		walletAddress = "";
		contractAddress = "";
		contractMethodName = "";
		contractName = "";
		responseDataVersion = "1";
	}

};

/**
* JSON batch request data
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FEtherlinkerBatchRequestData {

	GENERATED_USTRUCT_BODY()

	/**
	 * Unique sender identifier
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString userIndex;

	/**
	 * Link to the Integration Server
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString serverAddress;

	/**
	 * You should specify your custom access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString InfuraURL;

	/**
	 * Version of batch request data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString batchRequestDataVersion;

	/**
	 * List of requests to Ethereum blockchain
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	TArray<FEtherlinkerRequestData> etherlinkerRequestDataList;

	FEtherlinkerBatchRequestData() {
		senderId = "";
		userIndex = "0";
		batchRequestDataVersion = "1";
	}

};

/**
* JSON batch response data
*/
USTRUCT(BlueprintType)
struct ETHERLINKER_API FEtherlinkerBatchResponseData {

	GENERATED_USTRUCT_BODY()

	/**
	 * Unique sender identifier
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString senderId;

	/**
	 * User index in a project with multiplayer support
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString userIndex;

	/**
	 * Custom response data (can be transaction receipt, wallet balance, etc)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	FString data;

	/**
	 * Version of batch response data API
	 * Current version: 1
	 */
	UPROPERTY(BlueprintReadOnly, Category = "EtherManager")
	FString batchResponseDataVersion;

	/**
	 * List of responses from Ethereum blockchain
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EtherManager")
	TArray<FEtherlinkerResponseData> etherlinkerResponseDataList;

	FEtherlinkerBatchResponseData() {
		senderId = "";
		data = "";
		userIndex = "0";
		batchResponseDataVersion = "1";
	}
};

/**
* Event dispatcher declaration - OnResponseReceived
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseReceivedDelegate, FString, Result, FEtherlinkerResponseData, Data);

/**
* Event dispatcher declaration - OnBatchResponseReceived
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBatchResponseReceivedDelegate, FString, Result, FEtherlinkerBatchResponseData, Data);

/**
* Event dispatcher declaration - OnWalletAuthenticatorResponseReceived
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWalletAuthenticationResponseReceivedDelegate, FString, Result, FWalletAuthenticationResponse, Data);