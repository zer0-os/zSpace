// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "zSpace/Game/ZSGamePlayerController/ZSGamePlayerController.h"

#include "ZSWalletBalanceUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSWalletBalanceUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;
	
private:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess=true))
	UTextBlock * TextBlockBalance = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess=true))
	class UButton * ButtonGetBalance = nullptr; 
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess=true))
	UTextBlock * TextBlockButtonBalance = nullptr;

	
public:

	UFUNCTION(BlueprintCallable)
	void GetBalance(class UEthereumTerminalComponent * NewEthereumTerminalComponent);

private:
	
	class UZSEtherManager * GetZsEtherManager();

	class AZSGamePlayerController * GetPlayerController();

public:

	UFUNCTION()
	void ResponseReceived(FString  Result, struct FEtherlinkerResponseData Data);

	UFUNCTION()
	void OnClickGetBalance();

	
	
};





