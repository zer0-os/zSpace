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

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerLevel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* EditModeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcherEditMode = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* RemoveCharacterButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* DoneEditModeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcherDoneEditMode = nullptr;

	UPROPERTY(BlueprintReadWrite)
	uint8 bIsEditMode : 1;

public:
	UFUNCTION(BlueprintCallable)
	void SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);

protected:
	UFUNCTION()
	void OnClickedEditModeButton();

};
