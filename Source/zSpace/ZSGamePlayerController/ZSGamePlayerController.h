// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "OWSPlayerController.h"
#include "ZSGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSGamePlayerController : public AOWSPlayerController
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
	
};
