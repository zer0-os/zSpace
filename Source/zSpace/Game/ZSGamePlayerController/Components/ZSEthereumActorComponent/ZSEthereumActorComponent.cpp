// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSGamePlayerController/Components/ZSEthereumActorComponent/ZSEthereumActorComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "zSpace/Game/ZSGamePlayerController/Interfaces/EtherlinkerPCInterface/EtherlinkerPCInterface.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherManager/ZSEtherManager.h"

FZSWalletData::FZSWalletData()
{
	WalletAddress  = "";
	WalletPassword = "";
	WalletMnemonic = "";
	WalletPath     = "";
	WalletAuthType = "";
}

FZSWalletData FZSWalletData::operator=(const FWalletAuthenticationResponse& NewWalletAuthenticationResponse)
{
	WalletAddress  = NewWalletAuthenticationResponse.walletAddress;
	WalletPassword = NewWalletAuthenticationResponse.walletPassword;
	WalletMnemonic = NewWalletAuthenticationResponse.walletMnemonic;
	WalletPath     = NewWalletAuthenticationResponse.walletPath;
	WalletAuthType = "Mnemonic";
	return *this;
}

FZSWalletData FZSWalletData::operator=(const FZSWalletData& NewZSWalletData)
{
	WalletAddress  = NewZSWalletData.WalletAddress;
	WalletPassword = NewZSWalletData.WalletPassword;
	WalletMnemonic = NewZSWalletData.WalletMnemonic;
	WalletPath     = NewZSWalletData.WalletPath;
	WalletAuthType = NewZSWalletData.WalletAuthType;
	return *this;	
}

FZSWalletData FZSWalletData::operator=(const FWalletData& NewWalletData)
{
	WalletAddress  = NewWalletData.walletAddress;
	WalletPassword = NewWalletData.walletPassword;
	WalletMnemonic = NewWalletData.walletMnemonic;
	WalletPath     = NewWalletData.walletPath;
	WalletAuthType = NewWalletData.walletAuthType;
	return *this;	
}

FWalletData FZSWalletData::GetWalletDataByZSWalletData(const FZSWalletData & NewZSWalletData)
{
	FWalletData R_WalletData;
	R_WalletData.walletAddress = NewZSWalletData.WalletAddress;
	R_WalletData.walletMnemonic = NewZSWalletData.WalletMnemonic;
	R_WalletData.walletPassword = NewZSWalletData.WalletPassword;
	R_WalletData.walletPath = NewZSWalletData.WalletPath;
	R_WalletData.walletAuthType = NewZSWalletData.WalletAuthType;
	return R_WalletData;
}

bool FZSWalletData::IsNotEmptyMember() const
{
	bool R_Status = true;
	if(WalletAddress  == "")
	{
		UE_LOG(LogTemp, Error, TEXT("Error: wallet address can't be empty"));
		R_Status = false;	
	}
	if(WalletPath == "")
	{
		UE_LOG(LogTemp, Error, TEXT("Error: wallet path can't be empty"));
		R_Status = false;	
	}
	if(WalletAuthType == "")
	{
		
		UE_LOG(LogTemp, Error, TEXT("Error: wallet auth type can't be empty"));
		R_Status = false;	
	}
	return R_Status;
}

// Sets default values for this component's properties
UZSEthereumActorComponent::UZSEthereumActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	GetZSEtherlinkerRemoteWalletManager();
	GetZSEtherManager();
	
	// ...
}

void UZSEthereumActorComponent::GetZSEtherlinkerRemoteWalletManager()
{
	AActor * L_Actor = GetOwner();
	if(IsValid(L_Actor))
	{
		const bool L_IsImplemented = L_Actor->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
		if(L_IsImplemented)
		{
			ZSEtherlinkerRemoteWalletManager = IEtherlinkerPCInterface::Execute_GetZSEtherlinkerRemoteWalletManager(L_Actor);	
		}
	}
}

void UZSEthereumActorComponent::GetZSEtherManager()
{
	AActor * L_Actor = GetOwner();
	if(IsValid(L_Actor))
	{
		const bool L_IsImplemented = L_Actor->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
		if(L_IsImplemented)
		{
			ZSEtherManager = IEtherlinkerPCInterface::Execute_GetZSEtherManager(L_Actor);	
		}
	}
	
}


// Called when the game starts
void UZSEthereumActorComponent::BeginPlay()
{
	Super::BeginPlay();
	if(ROLE_Authority == GetOwnerRole())
	{
		BindEvents();
	}
	// ...
	
}



// Called every frame
void UZSEthereumActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UZSEthereumActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UZSEthereumActorComponent, WalletData);
	DOREPLIFETIME(UZSEthereumActorComponent, bIsWalletInitialized);
}

void UZSEthereumActorComponent::OnRwaResponseReceived(FString NewResult, FWalletAuthenticationResponse NewWalletAuthenticationResponse)
{
	const FString L_SenderID = GetSenderID();
	const FString L_SenderIDFromWalletResponse = NewWalletAuthenticationResponse.senderId;
	if(L_SenderID.Equals(L_SenderIDFromWalletResponse))
	{
		Client_ResponseReceivedWallet(NewResult, NewWalletAuthenticationResponse);
		const FString L_Success = "success";
		if(L_Success.Equals(NewResult))
		{
			const FString L_OperationType = NewWalletAuthenticationResponse.operationType;
			if(L_OperationType.Equals("getWalletData"))
			{
				FZSWalletData L_ZSWalletData;
				L_ZSWalletData = NewWalletAuthenticationResponse;	
				InitializeWallet(L_ZSWalletData);
			}
			if(L_OperationType.Equals("createUserAccount"))
			{
					
			}
		}
	}
}



FString UZSEthereumActorComponent::SenderID = "sender_playerstate_";

FString UZSEthereumActorComponent::GetSenderID_Implementation()
{
	FString R_SenderID = SenderID;
	APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
	checkf(nullptr != PC, TEXT("The PC is nullptr."));
	APlayerState * PS = PC->GetPlayerState<APlayerState>();
	checkf(nullptr != PS, TEXT("The PS is nullptr."));
	const int32 L_PlayerID = PS->GetPlayerId();
	R_SenderID.AppendInt(L_PlayerID);
	return R_SenderID;
	
}

void UZSEthereumActorComponent::InitializeWallet_Implementation(const FZSWalletData& NewZSWalletData)
{
	const bool L_IsValid = NewZSWalletData.IsNotEmptyMember();
	if(L_IsValid)
	{
		WalletData = NewZSWalletData;
		checkf(nullptr != ZSEtherManager, TEXT("The ZSEtherManager is nullptr.") );
		if(IsValid(ZSEtherManager))
		{
			const FWalletData L_WalletData = FZSWalletData::GetWalletDataByZSWalletData(NewZSWalletData);
			bIsWalletInitialized = true;
			ZSEtherManager->SetWalletData(L_WalletData);
		}
	}
}

bool UZSEthereumActorComponent::InitializeWallet_Validate(const FZSWalletData& NewZSWalletData)
{
	return true;
	
}

void UZSEthereumActorComponent::Client_ResponseReceivedWallet_Implementation(const FString& NewResult, const FWalletAuthenticationResponse& NewWalletAuthenticationResponse)
{
	UE_LOG(LogTemp, Warning, TEXT("ResponseReceivedWallet  == %s"), *NewResult)
	OnResponseReceivedWallet.Broadcast(NewResult, NewWalletAuthenticationResponse);
	
}

void UZSEthereumActorComponent::BindEvents()
{
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
	if(IsValid(ZSEtherlinkerRemoteWalletManager))
	{
		ZSEtherlinkerRemoteWalletManager->OnResponseReceivedDelegate.AddDynamic(this, &UZSEthereumActorComponent::OnRwaResponseReceived);
	}
}

void UZSEthereumActorComponent::CreateRemoteWallet_Implementation(const FString& NewLogin, const FString& NewPassword)
{
	if(ROLE_Authority == GetOwnerRole())
	{
		checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
		if(IsValid(ZSEtherlinkerRemoteWalletManager))
		{
			ZSEtherlinkerRemoteWalletManager->CreateRemoteWallet(NewLogin, NewPassword);
		}
	}
}

void UZSEthereumActorComponent::CreateRemoteWalletFromUI_Implementation(const FString& NewLogin, const FString& NewPassword)
{
	CreateRemoteWallet(NewLogin, NewPassword);
}


bool UZSEthereumActorComponent::CreateRemoteWallet_Validate(const FString& NewLogin, const FString& NewPassword)
{
	return true;	
}

void UZSEthereumActorComponent::GetWalletData_Implementation(const FString& NewLogin, const FString& NewPassword)
{
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
	if(IsValid(ZSEtherlinkerRemoteWalletManager))
	{
		ZSEtherlinkerRemoteWalletManager->GetWallet(NewLogin, NewPassword);	
	}
}

void UZSEthereumActorComponent::SetWalletDataWithMnemonic_Implementation(const FString& NewWalletAddress, const FString& NewMnemonic, const FString& NewPassword)
{
	checkf(nullptr != ZSEtherManager, TEXT("The ZSEtherManager is nullptr."));
	if(IsValid(ZSEtherManager))
	{
		FWalletAuthenticationResponse L_WalletAuthenticationResponse;
		const FString L_SenderID = GetSenderID();
		L_WalletAuthenticationResponse.walletAddress = NewWalletAddress;
		L_WalletAuthenticationResponse.walletMnemonic = NewMnemonic;
		L_WalletAuthenticationResponse.walletPassword = NewPassword;
		L_WalletAuthenticationResponse.senderId = L_SenderID;
		const FString L_Result = "getWalletData";
		OnRwaResponseReceived(L_Result, L_WalletAuthenticationResponse);
	}
}
