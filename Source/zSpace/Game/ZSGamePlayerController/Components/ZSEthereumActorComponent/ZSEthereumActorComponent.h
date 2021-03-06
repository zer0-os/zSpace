// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "Components/ActorComponent.h"
#include "Interface/ZSEthereumActorCompInterface.h"
#include "zSpace/EthereumTerminalComponent/EthereumTerminalComponent.h"

#include "ZSEthereumActorComponent.generated.h"


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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API UZSEthereumActorComponent : public UActorComponent , public IZSEthereumActorCompInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZSEthereumActorComponent();
	
private:

	UPROPERTY()
	class UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = nullptr;

	void GetZSEtherlinkerRemoteWalletManager();
	
	UPROPERTY()
	class UZSEtherManager * ZSEtherManager = nullptr;
	
	void  GetZSEtherManager();

	
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	FZSWalletData WalletData;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 bIsWalletInitialized:1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BindEvents();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
		
	UFUNCTION()
	void OnRwaResponseReceived(FString  NewResult, FWalletAuthenticationResponse  NewWalletAuthenticationResponse);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseReceivedWallet, FString, NewResult, FWalletAuthenticationResponse, NewWalletAuthenticationResponse);
	
	UPROPERTY(BlueprintAssignable)
	FResponseReceivedWallet OnResponseReceivedWallet;
	
	// [Client]
	UFUNCTION(Client, Reliable)	
	void Client_ResponseReceivedWallet(const FString &  NewResult, const FWalletAuthenticationResponse &  NewWalletAuthenticationResponse);

	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void InitializeWallet(const FZSWalletData & NewZSWalletData);

	
	UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
	void CreateRemoteWallet(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreateRemoteWalletFromUI(const FString & NewLogin, const FString & NewPassword);
	

	static FString SenderID;

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetSenderID();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetWalletData(const FString &NewLogin, const FString &NewPassword);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWalletDataWithMnemonic(const FString &NewWalletAddress, const FString & NewMnemonic, const FString &NewPassword);
	
public:
	
	UFUNCTION(BlueprintPure)
	bool CheckWalletInitialization() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetWalletAddress();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Execute(const FZSEtherlinkerRequestData &  NewEtherlinkerRequestData);

	UFUNCTION(BlueprintCallable)
	class UZSEtherManager* GetZsEtherManager();

	UFUNCTION(BlueprintCallable)
	class UZSEtherlinkerRemoteWalletManager * GetZsEtherlinkerRemoteWalletManager();

	UFUNCTION()
	void OnBatchResponseReceived(FString Result, FEtherlinkerBatchResponseData Data);
	
	 DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchResponseReceivedDelegate, FString, Result, FEtherlinkerBatchResponseData, Data);

	UPROPERTY(BlueprintAssignable)
	FOnBatchResponseReceivedDelegate OnBatchResponseReceivedDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResponseReceived);

	UPROPERTY(BlueprintAssignable)
	FResponseReceived OnResponseReceived;

	// [Client]
	UFUNCTION(Client, Reliable)
	void Client_OnBatchResponseReceived(const FString  & Result, const FEtherlinkerBatchResponseData & Data);
	
};
