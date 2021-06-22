// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "OWSPlayerController.h"
#include "Interfaces/EtherlinkerPCInterface/EtherlinkerPCInterface.h"
#include "ZSGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSGamePlayerController : public AOWSPlayerController, public IEtherlinkerPCInterface
{
	GENERATED_BODY()
	
public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	AZSGamePlayerController();

	UFUNCTION(BlueprintPure)
	bool CheckCharacterNameOwnedPlayerController(const FString & NewCharacterName);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ShowLoadingWidgetByWidget(TSubclassOf<class UUserWidget> NewUserWidgetSubClass);
	
	// [Server]
	UFUNCTION(BlueprintCallable)
	void ShowLoadingWidgetByCharacterName(const FString& NewCharacterName, TSubclassOf<class UUserWidget> NewUserWidgetSubClass);
	
	
	// [Client Owning]
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_HideLoadingWidget();

	// [Server]
	UFUNCTION(BlueprintCallable)
	void HideLoadingWidgetByCharacterName(const FString& NewCharacterName);

	UFUNCTION(BlueprintPure)
	FVector GetClosePlayerStart();

	// [Server]
	UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
	void ReTeleport();

protected:
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void HideOrShowGameplayWidget();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGameplayWidget();

private:
	// Wallet Start 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherManager * ZSEtherManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEthereumActorComponent * ZSEthereumActorComponent = nullptr;

	friend class UZSEtherlinkerRemoteWalletManager;
	
public:
	
	// Interface Etherlinker Start
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UZSEtherlinkerRemoteWalletManager *  GetZSEtherlinkerRemoteWalletManager();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UZSEtherManager * GetZSEtherManager();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UZSEthereumActorComponent	* GetZSEthereumActorComponent();
	
	// Interface Etherlinker End
	
	// Wallet end

	UFUNCTION(BlueprintCallable)
	void UseTerminal(class UEthereumTerminalComponent  * NewEthereumTerminalComponent);

	UFUNCTION(Server , Reliable, BlueprintCallable, WithValidation)
	void Server_UserTerminal(class UEthereumTerminalComponent  * NewEthereumTerminalComponent);

public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyGetAllUserCharacters, const TArray<struct FUserCharacter> &, UserCharacter);

	UPROPERTY(BlueprintAssignable)
	FNotifyGetAllUserCharacters OnNotifyGetAllUserCharacters;

	UFUNCTION(BlueprintCallable)
	void CallOnNotifyGetAllUserCharacters(const TArray<struct FUserCharacter> & NewUserCharacter);
	
};
