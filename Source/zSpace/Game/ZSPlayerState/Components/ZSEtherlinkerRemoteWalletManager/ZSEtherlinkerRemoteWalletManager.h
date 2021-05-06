// Copyright 2020 Sabre Dart Studios

#pragma once
#include "CoreMinimal.h"
#include "EtherlinkerRemoteWalletManager.h"
#include "ZSEthRemoteWalletInterface/ZSEthRemoteWalletInterface.h"
#include "ZSEtherlinkerRemoteWalletManager.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSEtherlinkerRemoteWalletManager : public UEtherlinkerRemoteWalletManager, public IZSEthRemoteWalletInterface
{
	GENERATED_BODY()

private:

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString SenderId = "sender_playerstate_";
	
public:

	virtual void BeginPlay() override;
	
	UZSEtherlinkerRemoteWalletManager();

	UFUNCTION(BlueprintPure)
	FString GetUserIndex();
	
	UFUNCTION(BlueprintCallable)
	void CreateRemoteWallet(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable)
	void GetWallet(const FString &NewLogin, const FString &NewPassword);


	UFUNCTION(BlueprintCallable)
	FWalletAuthenticationRequest GetWalletAuthenticationRequest(const FString &NewLogin, const FString &NewPassword);


	//  IZSEthRemoteWalletInterface  Start

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateBindings();

	//  IZSEthRemoteWalletInterface  End

	UFUNCTION()
	void ResponseReceivedDelegate(FString NewResult, FWalletAuthenticationResponse NewData);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResponseReceivedDelegate, FString, Result, FWalletAuthenticationResponse, Data);
	
	UPROPERTY(BlueprintAssignable)
	FOnResponseReceivedDelegate OnResponseReceivedDelegate;
	
};
