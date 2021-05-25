// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/Components/ZSEtherManager/ZSEtherManager.h"

UZSEtherManager::UZSEtherManager()
{
	OnResponseReceivedEvent.AddDynamic(this, &UZSEtherManager::Server_ResponseReceived);
}

void UZSEtherManager::Server_ResponseReceived(FString Result, FEtherlinkerResponseData Data)
{
	if(ROLE_Authority == GetOwnerRole())
	{
		Client_ResponseReceived(Result, Data);
	}
}

void UZSEtherManager::Client_ResponseReceived_Implementation(const FString & Result, const FEtherlinkerResponseData & Data)
{
	OnClientResponseReceivedEvent.Broadcast(Result, Data);
}

