// Copyright 2020 Sabre Dart Studios

#pragma once

#include "zSpace/Types/UITypes.h"

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* EnterNameBG = nullptr;

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

	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetCharacterInfoForUI)
	FCharacterInfoForUI CharacterInfoForUI;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetupWidget(const FCharacterInfoForUI& Data);
	
	UFUNCTION(BlueprintSetter)
	void SetPreviewCharacterPosition(EPreviewCharacterPosition NewValue);

	UFUNCTION(BlueprintGetter)
	const FCharacterInfoForUI& GetCharacterInfoForUI() const { return CharacterInfoForUI; };
	
	UFUNCTION(BlueprintCallable)
	void ChangeCreateCharacterMode();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayButtonsFadeInAnimation();

	UFUNCTION(BlueprintCallable)
	void SetBackGroundImage(class UTexture2D* NewTexture);

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
	void ChangeNormalMode();

	UFUNCTION(BlueprintPure)
	APreviewCharacter* GetPreviewCharacterByEnum(EPreviewCharacterPosition P_PreviewCharacterPosition) const;
};
