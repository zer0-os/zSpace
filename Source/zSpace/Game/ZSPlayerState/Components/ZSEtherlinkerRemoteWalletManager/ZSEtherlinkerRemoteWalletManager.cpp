// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"

#include "GameFramework/GameState.h"
#include "zSpace/Game/ZSGamePlayerController/Components/ZSEthereumActorComponent/ZSEthereumActorComponent.h"
#include "zSpace/Game/ZSGamePlayerController/Interfaces/EtherlinkerPCInterface/EtherlinkerPCInterface.h"
#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"

void UZSEtherlinkerRemoteWalletManager::BeginPlay()
{
	Super::BeginPlay();
	if(ROLE_Authority == GetOwnerRole())
	{
		UpdateBindings();
	}
	
}

UZSEtherlinkerRemoteWalletManager::UZSEtherlinkerRemoteWalletManager()
{
	
}

void UZSEtherlinkerRemoteWalletManager::CreateRemoteWallet(const FString& NewLogin, const FString& NewPassword)
{
	const FWalletAuthenticationRequest L_WalletAuthenticationRequest = GetWalletAuthenticationRequest(NewLogin, NewPassword);
	CreateUserAccount(L_WalletAuthenticationRequest);
}

void UZSEtherlinkerRemoteWalletManager::GetWallet(const FString& NewLogin, const FString& NewPassword)
{
	const FWalletAuthenticationRequest L_WalletAuthenticationRequest = GetWalletAuthenticationRequest(NewLogin, NewPassword);
	GetWalletData(L_WalletAuthenticationRequest);
}


FWalletAuthenticationRequest UZSEtherlinkerRemoteWalletManager::GetWalletAuthenticationRequest(const FString& NewLogin, const FString& NewPassword)
{
	FString L_UserIndex = GetUserIndex();
	FString L_SenderId = GetSender();
	L_SenderId.Append(L_UserIndex);
	FWalletAuthenticationRequest R_WalletAuthenticationRequest;
	R_WalletAuthenticationRequest.senderId = L_SenderId;
	R_WalletAuthenticationRequest.login = NewLogin;
	R_WalletAuthenticationRequest.password = NewPassword;
	R_WalletAuthenticationRequest.serverAddress = "";
	R_WalletAuthenticationRequest.userIndex = L_UserIndex;
	return R_WalletAuthenticationRequest;	
}

FString UZSEtherlinkerRemoteWalletManager::GetUserIndex()
{
	AZSPlayerState * PlayerState =  Cast<AZSPlayerState>(GetOwner());
	FString R_UserIndex = "";
	if(PlayerState)
	{
		const int32 L_PlayerID =  PlayerState->GetPlayerId();
		R_UserIndex.Append(FString::FormatAsNumber(L_PlayerID));
	}
	return R_UserIndex;
}

FString UZSEtherlinkerRemoteWalletManager::GetSender()
{
	FString R_Sender;
	AActor * L_Owner = GetOwner();
	if(IsValid(L_Owner))
	{
		const bool L_IsImplemented = L_Owner->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
		if(L_IsImplemented)
		{
			UZSEthereumActorComponent * L_ZSEthereumActorComponent = IEtherlinkerPCInterface::Execute_GetZSEthereumActorComponent(L_Owner);
			if(IsValid(L_ZSEthereumActorComponent))
			{
				const bool L_IsImp = L_ZSEthereumActorComponent->GetClass()->ImplementsInterface(UZSEthereumActorCompInterface::StaticClass());
				if(L_IsImp)
				{
					R_Sender = IZSEthereumActorCompInterface::Execute_GetSenderID(L_ZSEthereumActorComponent);
				}
			}
		}
	}
	return R_Sender;
}

void UZSEtherlinkerRemoteWalletManager::UpdateBindings_Implementation()
{
	AActor * L_Owner = GetOwner();	
	if(IsValid(L_Owner))
	{
		const bool L_IsImplemented = L_Owner->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
		if(L_IsImplemented)
		{
			UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = IEtherlinkerPCInterface::Execute_GetZSEtherlinkerRemoteWalletManager(L_Owner);
			if(ZSEtherlinkerRemoteWalletManager)
			{
				ZSEtherlinkerRemoteWalletManager->OnResponseReceivedEvent.AddDynamic(this, &UZSEtherlinkerRemoteWalletManager::ResponseReceivedDelegate);
			}
		}
	}
}

void UZSEtherlinkerRemoteWalletManager::ResponseReceivedDelegate(FString NewResult, FWalletAuthenticationResponse NewData)
{
	UE_LOG(LogTemp, Warning, TEXT("SenderID = %s, WalletAddress = %s, WalletMnemonic = %s"), *NewData.senderId, *NewData.walletAddress, *NewData.walletMnemonic);
	OnResponseReceivedDelegate.Broadcast(NewResult, NewData);
}
