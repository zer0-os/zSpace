// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"
#include "../Interfaces/UIResolutionInterface.h"
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


public:
	UPROPERTY(BlueprintReadOnly)
	class USelectCharacterBoxUserWidget* SelectedCharacterBoxUserWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterMiddleCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterRightCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterLeftCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* SelectButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* CreateNewCharacterButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaPlayer* MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaSource* MediaSource = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> SelectCharacterBoxSubClass;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void CreateCharacterSelectBox(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo);

public:
	EWidgetType GetWidgetType_Implementation() override;
};
