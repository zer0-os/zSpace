// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"


#include "Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "Components/ZSEtherManager/ZSEtherManager.h"
#include "zSpace/Game/ZSGameState/ZSGameState.h"

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
	const FString L_Success = "success";
	const FString L_SenderID = GetSenderID();
	const FString L_SenderIDFromWalletResponse = NewWalletAuthenticationResponse.senderId;
	if(L_SenderID.Equals(L_SenderIDFromWalletResponse))
	{
		if(L_Success.Equals(NewResult))
		{
						
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



