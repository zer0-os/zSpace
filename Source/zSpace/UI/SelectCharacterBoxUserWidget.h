// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "../Types/UITypes.h"
#include "Blueprint/UserWidget.h"
#include "SelectCharacterBoxUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API USelectCharacterBoxUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerLevel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* BackgroundImage = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);
};
