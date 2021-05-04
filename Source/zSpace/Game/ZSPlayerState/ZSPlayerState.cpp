// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSPlayerState/ZSPlayerState.h"
#include "Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "Components/ZSEtherManager/ZSEtherManager.h"

AZSPlayerState::AZSPlayerState()
{
	ZSEtherManager = CreateDefaultSubobject<UZSEtherManager>(TEXT("UZSEtherManager"));
	checkf(nullptr != ZSEtherManager, TEXT("The EtherManager is nullptr."));
	AddOwnedComponent(ZSEtherManager);

	ZSEtherlinkerRemoteWalletManager = CreateDefaultSubobject<UZSEtherlinkerRemoteWalletManager>(TEXT("ZSEtherlinkerRemoteWalletManager"));
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The EtherlinkerRemoteWalletManager is nulltrp."));
	AddOwnedComponent(ZSEtherlinkerRemoteWalletManager);
	
}
