// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"
#include "OWSPlayerController.h"
#include "SelectCharacterBoxUserWidget.h"
#include "../Interfaces/UIResolutionInterface.h"

#include "SelectCharacterUserWidget.generated.h"

UENUM(BlueprintType)
enum class EChangeCharacterDirection : uint8
{
	None,
	ToRight,
	ToLeft,
};

/**
 * 
 */
UCLASS()
class ZSPACE_API USelectCharacterUserWidget : public UUserWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void ToPreviousMenu();

	UFUNCTION(BlueprintCallable)
	void ShowCreateNewCharacterWidget(TSubclassOf<class UUserWidget> Class);
	
	UFUNCTION(BlueprintCallable)
	void HideCreateNewCharacterWidget();

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterMiddleBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterRightBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterLeftBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* AnimationBorderLeft = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UUserWidget* CreateNewCharacterWidget = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> SelectCharacterBoxSubClass;

	UPROPERTY(BlueprintReadWrite, Transient, BlueprintSetter=SetMainCharacterBox)
	class UBorder* MainCharacterBox = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	class UBorder* LeftCharacterBox = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	class UBorder* RightCharacterBox = nullptr;

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void CreateCharacterSelectBox(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo, class UBorder* ParentBorder);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void ShowCharacters(const TArray<FUserCharacter>& UserCharacters, const int32 CurrentCharacterIndex);

	UFUNCTION(BlueprintPure)
	class USelectCharacterBoxUserWidget* GetSelectedCharacterBox() const;

	UFUNCTION(BlueprintSetter)
	void SetMainCharacterBox(class UBorder* NewValue);

	UFUNCTION(BlueprintCallable)
	void UpdateBorderToRight();
	
	UFUNCTION(BlueprintCallable)
	void UpdateBorderToLeft();

	UFUNCTION(BlueprintPure)
	TArray<UBorder*> GetBoxBorders() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	EChangeCharacterDirection LastChangeCharacterDirection = EChangeCharacterDirection::None;
};
