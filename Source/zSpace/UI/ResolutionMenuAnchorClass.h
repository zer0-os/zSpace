// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "ZSCustomButton.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"

#include "ResolutionMenuAnchorClass.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UResolutionMenuAnchorClass : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void OnSelectResolution(class UZSCustomButton* Button);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UButtonStyleByResolution* ButtonStyleByResolution = nullptr;

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* VerticalBox_Resolutions = nullptr;
	
	UPROPERTY()
	TMap<EResolution, class UButton*> ResolutionAndButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UResolutionAndWidgetDataAsset* ResolutionAndWidgetDataAsset = nullptr;

	UPROPERTY()
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	UZSCustomButton* CreateButton(const FButtonStyle ButtonStyle, EResolution Resolution);

	UFUNCTION(BlueprintCallable)
	void SetResolutionAndWidgetDataAsset(class UResolutionAndWidgetDataAsset* NewValue);
	
	void CreateDefaultButtons();
};
