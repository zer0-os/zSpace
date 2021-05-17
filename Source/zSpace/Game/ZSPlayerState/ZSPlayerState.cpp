// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"


#include "Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "Net/UnrealNetwork.h"


AZSPlayerState::AZSPlayerState()
{
	
}

void AZSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AZSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

