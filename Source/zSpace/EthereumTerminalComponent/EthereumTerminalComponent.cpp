// Copyright 2020 Sabre Dart Studios


#include "zSpace/EthereumTerminalComponent/EthereumTerminalComponent.h"
#include "zSpace/Game/ZSGamePlayerController/Components/ZSEthereumActorComponent/ZSEthereumActorComponent.h"
#include "EtherlinkerFunctionLibrary.h"
#include "EtherlinkerTypes.h"
#include "EthereumTerminalDataAsset/EthereumTerminalDataAsset.h"
#include "Math/UnitConversion.h"
#include "zSpace/Game/ZSGamePlayerController/Interfaces/EtherlinkerPCInterface/EtherlinkerPCInterface.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherManager/ZSEtherManager.h"

FEtherlinkerRequestData FZSEtherlinkerRequestData::GetEtherlinkerRequestData() const
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
	for(EEthereumParamTypes IterEthParamType : ContractMethodParamTypes)
	{
		 FString R_StrParam;
		 UEnum::GetValueAsString<EEthereumParamTypes>(IterEthParamType, R_StrParam);
		 R_StrParam = R_StrParam.Replace(TEXT("EEthereumParamTypes::"), TEXT(""));
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
	R_StrOperationType = R_StrOperationType.Replace(TEXT("EEthOperationType::"), TEXT(""));
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
	bCallCompleted = true;
	// ...
}


// Called when the game starts
void UEthereumTerminalComponent::BeginPlay()
{
	Super::BeginPlay();
	SetZSEtherlinkerRequestData();
	// ...
	
}


// Called every frame
void UEthereumTerminalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEthereumTerminalComponent::ShowWalletBalanceWidget()
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		if(WalletBalance == nullptr)
		{
			const TSubclassOf<UUserWidget> L_WalletBalanceClass = WalletBalanceClassPtr.LoadSynchronous();
			if(L_WalletBalanceClass)
			{
				APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
				checkf(nullptr != PC, TEXT("The PC is nullptr."))
				WalletBalance = CreateWidget<UUserWidget>(PC, L_WalletBalanceClass);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The WalletBalanceClassPtr is nullptr. Please set WalletbalanceClass."));
			}
		}
		if(WalletBalance)
		{
			if(false == WalletBalance->IsInViewport())
			{
				WalletBalance->AddToViewport(WalletZOrder);	
			}
		}
	}
}

void UEthereumTerminalComponent::HideWalletBalanceWidget()
{
	if(WalletBalance)
	{
		WalletBalance->RemoveFromViewport();
	}

}

UZSEthereumActorComponent * UEthereumTerminalComponent::GetZsEthereumActorComponent(APawn * NewInteractor)
{
	UZSEthereumActorComponent * R_ZSEthereumActorComponent = nullptr;
	if(IsValid(NewInteractor))
	{
		AController * PC =	NewInteractor->GetController();
		if(IsValid(PC))
		{
			const bool IsImplemented = PC->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
			if(IsImplemented)
			{
				R_ZSEthereumActorComponent = IEtherlinkerPCInterface::Execute_GetZSEthereumActorComponent(PC);	
				UE_LOG(LogTemp, Log, TEXT("Got ZSEthereumActorComponent from PC"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The PlayerController doesn't implemented IEtherlinkerPCInterface"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The NewInteractor is nullptr. Please Check "));
	}
	return R_ZSEthereumActorComponent;
}

UZSEtherManager* UEthereumTerminalComponent::GetZsEtherManager(APawn* NewInteractor)
{
	UZSEtherManager * R_ZsEtherManager = nullptr;
	if(IsValid(NewInteractor))
	{
		AController * PC =	NewInteractor->GetController();
		if(IsValid(PC))
		{
			const bool IsImplemented = PC->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
			if(IsImplemented)
			{
				R_ZsEtherManager = IEtherlinkerPCInterface::Execute_GetZSEtherManager(PC);	
				UE_LOG(LogTemp, Log, TEXT("Got ZSEtherManager from PC"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The PlayerController doesn't implemented IEtherlinkerPCInterface"));
			}
		}
	}
	return R_ZsEtherManager;
}

void UEthereumTerminalComponent::SetZSEtherlinkerRequestData()
{
	if(ROLE_Authority == GetOwnerRole())
	{
		if(EthereumTerminalDataAsset)
		{
			EtherlinkerRequestData = EthereumTerminalDataAsset->GetZSEtherlinkerRequestData();
		}
	}
}

void UEthereumTerminalComponent::ResponseReceived(FString Result, FEtherlinkerResponseData Data)
{
	if(ROLE_Authority == GetOwnerRole())
	{
		UE_LOG(LogTemp, Warning, TEXT("ZSpace: Result = %s"), *Result);
		FString L_Out;
		FJsonObjectConverter::UStructToJsonObjectString(Data, L_Out);
		UE_LOG(LogTemp, Warning, TEXT("ZSpace: Data = %s"), *L_Out);
		bCallCompleted = true;
	}
}


void UEthereumTerminalComponent::Use(class APawn * NewInteractor)
{
	
	if(ROLE_Authority == GetOwnerRole())
	{
		if(bCanBeUsed)
		{
			UZSEtherManager * L_ZsEtherManager = GetZsEtherManager(NewInteractor);
			checkf(nullptr != L_ZsEtherManager, TEXT("The L_ZsEtherManagetr is nullptr."));
			if(L_ZsEtherManager)
			{
				L_ZsEtherManager->OnResponseReceivedEvent.AddUniqueDynamic(this, &UEthereumTerminalComponent::ResponseReceived)	;
			}
			UZSEthereumActorComponent * L_ZSEthereumActorComponent = GetZsEthereumActorComponent(NewInteractor);
			checkf(nullptr != L_ZSEthereumActorComponent, TEXT("The L_ZSEthereumActorComponent is nullptr."));
			if(L_ZSEthereumActorComponent)
			{
				const bool L_IsWalletInitialization  = L_ZSEthereumActorComponent->CheckWalletInitialization();
				if(L_IsWalletInitialization || EEthOperationType::createWallet == EtherlinkerRequestData.OperationType )
				{
					if(bCallCompleted)
					{
						bCallCompleted = false;
						const FString L_WalletAddress = L_ZSEthereumActorComponent->GetWalletAddress();
						if(UEtherlinkerFunctionLibrary::IsWalletAddressValid(L_WalletAddress) && bUseCurrentWalletAddressAsFirstParameter)
						{
							EtherlinkerRequestData.ContractMethodParams.Insert(L_WalletAddress, 0);
						}
						L_ZSEthereumActorComponent->Execute(EtherlinkerRequestData);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Error: Wallet is not initialized"));	
				}
			}
		}
	}
}

