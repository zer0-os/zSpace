// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"

#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"

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
	FString L_SenderId = "sender_playerstate_";
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
		int32 L_UserIndex = PlayerState->UserIndex;
		R_UserIndex.Append(FString::FormatAsNumber(L_UserIndex));
	}
	return R_UserIndex;
}
