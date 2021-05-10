// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "OWSPlayerState.h"
#include "Components/ZSEtherManager/ZSEtherManagerHolderInterface/ZSEtherManagerHolderInterface.h"
#include "ZSPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FZSWalletData
{
	GENERATED_BODY()

	FZSWalletData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WalletAddress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WalletPassword;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WalletMnemonic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WalletPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WalletAuthType;

	FZSWalletData operator=(const FWalletAuthenticationResponse & NewWalletAuthenticationResponse);

	FZSWalletData operator=(const FZSWalletData & NewZSWalletData );
	
	FZSWalletData operator=(const FWalletData  & NewWalletData );
	
	static FWalletData GetWalletDataByZSWalletData(const FZSWalletData & NewZSWalletData);

	bool IsNotEmptyMember() const;
	
};

//void operator=(FWalletData &  NewWalletData  ,  FZSWalletData & NewZSWalletData);


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

	friend class UZSEtherlinkerRemoteWalletManager;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	FZSWalletData WalletData;

	UPROPERTY(Replicated)
	uint8 bIsWalletInitialized:1;


public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	

	static FString SenderID;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetSenderID();
	
private:


	void BindEvents();
	
public:

	// Interface  IZSEtherManagerHolderInterface Start
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteUpdateBindings(class UActorComponent * NewEtherInteractor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UZSEtherlinkerRemoteWalletManager * GetEtherlinkerRemoteWalletManager();

	
	// Interface  IZSEtherManagerHolderInterface End

	// [Server]
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_ExecuteUpdateBindingsForUsableActors(class UActorComponent * NewEtherInteractor);

	UFUNCTION()
	void OnRwaResponseReceived(FString  NewResult, FWalletAuthenticationResponse  NewWalletAuthenticationResponse);

	UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
	void CreateRemoteWallet(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ServerCreateRemoteWalletFromUI(const FString & NewLogin, const FString & NewPassword);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void InitializeWallet(const FZSWalletData & NewZSWalletData);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseReceivedWallet, FString, NewResult, FWalletAuthenticationResponse, NewWalletAuthenticationResponse);

	UPROPERTY(BlueprintAssignable)
	FResponseReceivedWallet OnResponseReceivedWallet;
	
	// [Client]
	UFUNCTION(Client, Reliable)	
	void Client_ResponseReceivedWallet(const FString &  NewResult, const FWalletAuthenticationResponse &  NewWalletAuthenticationResponse);

	

};
