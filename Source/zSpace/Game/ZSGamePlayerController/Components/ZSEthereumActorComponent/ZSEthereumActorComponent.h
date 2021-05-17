// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "Components/ActorComponent.h"
#include "Interface/ZSEthereumActorCompInterface.h"
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

	UPROPERTY(Replicated)
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
	
	// [Server]
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_ExecuteUpdateBindingsForUsableActors(class UActorComponent * NewEtherInteractor);

	static FString SenderID;

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetSenderID();

	

};
