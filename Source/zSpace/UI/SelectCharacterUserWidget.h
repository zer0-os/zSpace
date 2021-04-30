// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"
#include "OWSPlayerController.h"
#include "SelectCharacterBoxUserWidget.h"
#include "../Interfaces/UIResolutionInterface.h"
#include "zSpace/Components/ManageWidgetsResolution.h"

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

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterMiddleBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterRightBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* SelectCharacterLeftBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* AnimationBorderLeft = nullptr;
	
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

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateBorderToRight();
	
	UFUNCTION(BlueprintCallable)
	void UpdateBorderToLeft();

	UFUNCTION(BlueprintPure)
	TArray<UBorder*> GetBoxBorders() const;

	UPROPERTY(BlueprintReadOnly)
	EChangeCharacterDirection LastChangeCharacterDirection = EChangeCharacterDirection::None;

	UPROPERTY(BlueprintReadOnly)
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	UFUNCTION(BlueprintCallable)
	virtual void PlayAnimationChangeCharacter(class UWidgetAnimation* ChangeAnimation, EChangeCharacterDirection AnimationDirection);

	UFUNCTION(BlueprintCallable)
	void ResetBoxesTransform();

	virtual void SetPreviewCharacterPositionByCharacterBox(USelectCharacterBoxUserWidget* Widget);
};
