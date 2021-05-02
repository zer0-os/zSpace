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

	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerLevel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* BackgroundImage = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* CharacterRenderImage = nullptr;

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
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSCustomButton* NextCharacterMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSCustomButton* PreviousCharacterMesh = nullptr;

	UPROPERTY(BlueprintReadWrite)
	uint8 bIsEditMode : 1;

	UPROPERTY(BlueprintReadWrite)
	uint8 bIsCreateCharacterMode : 1;

	UPROPERTY()
	class APreviewCharacter* PreviewCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	class USelectCharacterUserWidget* SelectCharacterUserWidget = nullptr;

protected:
	UPROPERTY(BlueprintSetter=SetPreviewCharacterPosition, BlueprintReadWrite)
	EPreviewCharacterPosition PreviewCharacterPosition;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);
	
	UFUNCTION(BlueprintSetter)
	void SetPreviewCharacterPosition(EPreviewCharacterPosition NewValue);

protected:
	UFUNCTION()
	void OnClickedEditModeButton();
	
	UFUNCTION()
	void OnClickedNextCharacterMesh();
	
	UFUNCTION()
	void OnClickedPreviousCharacterMesh();

	UFUNCTION()
	void OnClickedDoneEditModeButton();
	
	UFUNCTION(BlueprintCallable)
	void ChangeCreateCharacterMode();

	UFUNCTION(BlueprintCallable)
	void ChangeNormalMode();

	UFUNCTION(BlueprintPure)
	APreviewCharacter* GetPreviewCharacterByEnum(EPreviewCharacterPosition P_PreviewCharacterPosition) const;
};
