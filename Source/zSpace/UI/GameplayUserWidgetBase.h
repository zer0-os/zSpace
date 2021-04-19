// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Interfaces/UIResolutionInterface.h"
#include "../Types/UITypes.h"
#include "GameplayUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UGameplayUserWidgetBase : public UUserWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintCosmetic)
	void SetPlayerName(const FText& PlayerName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SetHealth(const float CurrentValue, const float MaxValue);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SetStamina(const float CurrentValue, const float MaxValue);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SetPlayerCorrectName();

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HealthProgressBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* HealthTextValue = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* StaminaProgressBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UChatUserWidget* ChatWidget = nullptr;
};
