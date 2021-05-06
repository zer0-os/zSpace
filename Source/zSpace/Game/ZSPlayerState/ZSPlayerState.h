// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "OWSPlayerState.h"
#include "Components/ZSEtherManager/ZSEtherManagerHolderInterface/ZSEtherManagerHolderInterface.h"

#include "ZSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSPlayerState : public AOWSPlayerState, public IZSEtherManagerHolderInterface
{
	GENERATED_BODY()
	
public:
	
	AZSPlayerState();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherManager * ZSEtherManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	int32 UserIndex = -1;

	friend class UZSEtherlinkerRemoteWalletManager;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	FWalletData WalletData;


public:

	static FString SenderID;

	FString GetSenderID();
	
private:


	void BindEvents();
	
public:

	// Interface  IZSEtherManagerHolderInterface Start
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteUpdateBindings(class UActorComponent * NewEtherInteractor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UZSEtherlinkerRemoteWalletManager * GetEtherlinkerRemoteWalletManager();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetUserIndex();
	
	// Interface  IZSEtherManagerHolderInterface End

	// [Server]
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_ExecuteUpdateBindingsForUsableActors(class UActorComponent * NewEtherInteractor);

	UFUNCTION()
	void OnRwaResponseReceived(FString  NewResult, FWalletAuthenticationResponse  NewWalletAuthenticationResponse);

	UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
	void CreateRemoteWallet(const FString &NewLogin, const FString &NewPassword);

};
