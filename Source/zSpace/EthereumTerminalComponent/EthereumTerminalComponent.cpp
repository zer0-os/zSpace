// Copyright 2020 Sabre Dart Studios


#include "zSpace/EthereumTerminalComponent/EthereumTerminalComponent.h"
#include "EtherlinkerTypes.h"

FEtherlinkerRequestData FZSEtherlinkerRequestData::GetEtherlinkerRequestData()
{
	FEtherlinkerRequestData R_EtherlinkerRequestData;
	R_EtherlinkerRequestData.senderId = SenderID;
	R_EtherlinkerRequestData.userIndex = UserIndex;
	R_EtherlinkerRequestData.serverAddress = ServerAddress;
	R_EtherlinkerRequestData.walletAddress = WalletAddress;
	R_EtherlinkerRequestData.contractAddress = ContractAddress;
	R_EtherlinkerRequestData.contractMethodName = ContractMethodName;
	R_EtherlinkerRequestData.contractName = ContractName;
	R_EtherlinkerRequestData.contractMethodParams = ContractMethodParams;
	for(EEthereumParamTypes IterEthParamType : TEnumRange<EEthereumParamTypes>())
	{
		 FString R_StrParam;
		 UEnum::GetValueAsString<EEthereumParamTypes>(IterEthParamType, R_StrParam);
		 R_EtherlinkerRequestData.contractMethodParamTypes.Add(R_StrParam);
	}
	R_EtherlinkerRequestData.convertResultFromWeiToEth = ConvertResultFromWeiToEth;
	R_EtherlinkerRequestData.gasPrice = GasPrice;
	R_EtherlinkerRequestData.gasLimit = GasLimit;
	R_EtherlinkerRequestData.InfuraURL = InfuraURL;
	R_EtherlinkerRequestData.receiverAddress = ReceiverAddress;
	R_EtherlinkerRequestData.ethAmountToSend = EthAmountToSend;
	FString R_StrOperationType;
	UEnum::GetValueAsString<EEthOperationType>(OperationType, R_StrOperationType);
	R_EtherlinkerRequestData.operationType = R_StrOperationType;
	return R_EtherlinkerRequestData;
}

// Sets default values for this component's properties
UEthereumTerminalComponent::UEthereumTerminalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bCanBeUsed = true;
	EtherlinkerRequestData.SenderID = "senderId";
	EtherlinkerRequestData.GasPrice = "22000000000";
	EtherlinkerRequestData.GasLimit = "4300000";
	EtherlinkerRequestData.ConvertResultFromWeiToEth = "false";
	
	// ...
}


// Called when the game starts
void UEthereumTerminalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEthereumTerminalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

