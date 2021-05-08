// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"

#include "GameFramework/GameState.h"
#include "zSpace/Game/ZSGameState/ZSGameState.h"
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
	FString L_SenderId = AZSPlayerState::SenderID;
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

void UZSEtherlinkerRemoteWalletManager::UpdateBindings_Implementation()
{
	    // TODO the code must be optimized after testing.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&] ()
		{
			AZSGameState * L_ZSGameState = Cast<AZSGameState>(UGameplayStatics::GetGameState(this));
			if(L_ZSGameState)
			{
				if( L_ZSGameState->PlayerArray.Num() > 0)
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					for(APlayerState * IterPlayerState : L_ZSGameState->PlayerArray)
					{
						if(IsValid(IterPlayerState))
						{
							const bool L_IsImplemented = IterPlayerState->GetClass()->ImplementsInterface(UZSEtherManagerHolderInterface::StaticClass());
							if(L_IsImplemented)
							{
								UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = IZSEtherManagerHolderInterface::Execute_GetEtherlinkerRemoteWalletManager(IterPlayerState);
								if(ZSEtherlinkerRemoteWalletManager)
								{
									// TODO more build on client ????.
									ZSEtherlinkerRemoteWalletManager->OnResponseReceivedEvent.AddDynamic(this, &UZSEtherlinkerRemoteWalletManager::ResponseReceivedDelegate);
								}
							}
						}
					}
					
				}
					
			}
		}, 0.3, true );
}

void UZSEtherlinkerRemoteWalletManager::ResponseReceivedDelegate(FString NewResult, FWalletAuthenticationResponse NewData)
{
	AZSGameState * L_ZSGameState = Cast<AZSGameState>(UGameplayStatics::GetGameState(this));
	if(L_ZSGameState)
	{
		TArray<APlayerState *> L_PlayerState = L_ZSGameState->PlayerArray;
		
		for(APlayerState * IterPlayerState : L_PlayerState )
		{
			if(IsValid(IterPlayerState))
			{
				const int32 L_UserIndexFromNewData = FCString::Atoi(*NewData.userIndex);
				const int32 L_UserIndex = IterPlayerState->GetPlayerId();
				if(L_UserIndex == L_UserIndexFromNewData)
				{
					OnResponseReceivedDelegate.Broadcast(NewResult, NewData);
				}
			}
		}
	}
}
