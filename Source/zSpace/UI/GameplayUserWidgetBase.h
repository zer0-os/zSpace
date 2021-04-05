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

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintCosmetic)
	void SetPlayerName(const FText& PlayerName);

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HealthProgressBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* StaminaProgressBar = nullptr;

// Interface UIResolutionInterface
protected:
	virtual EWidgetType GetWidgetType_Implementation() override;
	
};
