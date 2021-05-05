// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "EtherlinkerRemoteWalletManager.h"
#include "ZSEtherlinkerRemoteWalletManager.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSEtherlinkerRemoteWalletManager : public UEtherlinkerRemoteWalletManager
{
	GENERATED_BODY()
	
public:
	
	UZSEtherlinkerRemoteWalletManager();

	UFUNCTION(BlueprintPure)
	FString GetUserIndex();
	
	UFUNCTION(BlueprintCallable)
	void CreateRemoteWallet(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable)
	void GetWallet(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable)
	FWalletAuthenticationRequest GetWalletAuthenticationRequest(const FString &NewLogin, const FString &NewPassword);

	
};
