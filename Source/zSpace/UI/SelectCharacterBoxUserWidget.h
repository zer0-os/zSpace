// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "../Types/UITypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"

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
	class UZSpaceButton* SelectButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* EditModeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcherEditMode = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* RemoveCharacterButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSpaceButton* DoneEditModeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcherDoneEditMode = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSpaceButton* CreateCharacterDone = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* CreateCharacterNameSwitcher = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* NewCharacterName = nullptr;

	UPROPERTY(BlueprintReadWrite)
	uint8 bIsEditMode : 1;

	UPROPERTY(BlueprintReadWrite)
	uint8 bIsCreateCharacterMode : 1;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);

protected:
	UFUNCTION()
	void OnClickedEditModeButton();
	
	UFUNCTION(BlueprintCallable)
	void ChangeCreateCharacterMode();

	UFUNCTION(BlueprintCallable)
	void ChangeNormalMode();
	
};
