// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"
#include "../Interfaces/UIResolutionInterface.h"
#include "OWSPlayerController.h"
#include "SelectCharacterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API USelectCharacterUserWidget : public UUserWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void ToPreviousMenu();

	UFUNCTION(BlueprintCallable)
	void ShowCreateNewCharacterWidget(TSubclassOf<class UUserWidget> Class);
	
	UFUNCTION(BlueprintCallable)
	void HideCreateNewCharacterWidget();

public:
	UPROPERTY(BlueprintReadOnly)
	class USelectCharacterBoxUserWidget* SelectedCharacterBoxUserWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterMiddleCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterRightCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterLeftCanvas = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UUserWidget* CreateNewCharacterWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaPlayer* MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaSource* MediaSource = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> SelectCharacterBoxSubClass;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void CreateCharacterSelectBox(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void ShowCharacters(TArray<FUserCharacter>& UserCharacters, int32 CurrentCharacterIndex);

public:
	EWidgetType GetWidgetType_Implementation() override;
};
