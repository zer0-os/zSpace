// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"


#include "Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "Components/ZSEtherManager/ZSEtherManager.h"
#include "Net/UnrealNetwork.h"
#include "zSpace/Game/ZSGameState/ZSGameState.h"

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



AZSPlayerState::AZSPlayerState()
{
	ZSEtherManager = CreateDefaultSubobject<UZSEtherManager>(TEXT("UZSEtherManager"));
	checkf(nullptr != ZSEtherManager, TEXT("The EtherManager is nullptr."));
	AddOwnedComponent(ZSEtherManager);

	ZSEtherlinkerRemoteWalletManager = CreateDefaultSubobject<UZSEtherlinkerRemoteWalletManager>(TEXT("ZSEtherlinkerRemoteWalletManager"));
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The EtherlinkerRemoteWalletManager is nulltrp."));
	AddOwnedComponent(ZSEtherlinkerRemoteWalletManager);
	
}

void AZSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	BindEvents();
}


FString AZSPlayerState::SenderID = "sender_playerstate_";


void AZSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZSPlayerState, bIsWalletInitialized);
}

FString AZSPlayerState::GetSenderID()
{
	FString R_SenderID = AZSPlayerState::SenderID;
	R_SenderID.AppendInt(UserIndex);
	return R_SenderID;
}

void AZSPlayerState::BindEvents()
{
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
	if(IsValid(ZSEtherlinkerRemoteWalletManager))
	{
		ZSEtherlinkerRemoteWalletManager->OnResponseReceivedDelegate.AddDynamic(this, &AZSPlayerState::OnRwaResponseReceived);
	}
}

UZSEtherlinkerRemoteWalletManager* AZSPlayerState::GetEtherlinkerRemoteWalletManager_Implementation()
{
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
	return ZSEtherlinkerRemoteWalletManager;
}

int32 AZSPlayerState::GetUserIndex_Implementation()
{
	return UserIndex;
}

void AZSPlayerState::ExecuteUpdateBindings_Implementation(UActorComponent* NewEtherInteractor)
{
		
}

void AZSPlayerState::Server_ExecuteUpdateBindingsForUsableActors_Implementation(UActorComponent* NewEtherInteractor)
{
	if(ROLE_Authority == GetLocalRole())
	{
		if(IsValid(NewEtherInteractor))
		{
			UZSEtherlinkerRemoteWalletManager * L_ZSEtherlinkerRemoteWalletManager = Cast<UZSEtherlinkerRemoteWalletManager>(NewEtherInteractor);
		}
	}
}

bool AZSPlayerState::Server_ExecuteUpdateBindingsForUsableActors_Validate(UActorComponent* NewEtherInteractor)
{
	return true;	
}


void AZSPlayerState::OnRwaResponseReceived(FString NewResult, FWalletAuthenticationResponse NewWalletAuthenticationResponse)
{
	const FString L_SenderID = GetSenderID();
	const FString L_SenderIDFromWalletResponse = NewWalletAuthenticationResponse.senderId;
	if(L_SenderID.Equals(L_SenderIDFromWalletResponse))
	{
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
bool AZSPlayerState::InitializeWallet_Validate(const FZSWalletData& NewZSWalletData)
{
	return  true;
}

void AZSPlayerState::InitializeWallet_Implementation(const FZSWalletData & NewZSWalletData)
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


bool AZSPlayerState::CreateRemoteWallet_Validate(const FString& NewLogin, const FString& NewPassword)
{
	return  true;
}

void AZSPlayerState::CreateRemoteWallet_Implementation(const FString& NewLogin, const FString& NewPassword)
{
	if(ROLE_Authority == GetLocalRole())
	{
		checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr."));
		if(IsValid(ZSEtherlinkerRemoteWalletManager))
		{
			ZSEtherlinkerRemoteWalletManager->CreateRemoteWallet(NewLogin, NewPassword);
		}
	}
}

void AZSPlayerState::ServerCreateRemoteWalletFromUI_Implementation(const FString& NewLogin, const FString& NewPassword)
{
	CreateRemoteWallet(NewLogin, NewPassword);
}

