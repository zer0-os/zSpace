// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"
#include "Components/TimelineComponent.h"
#include "../Interfaces/UIResolutionInterface.h"

#include "RegisterUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API URegisterUserWidgetBase : public UOWSLoginWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void NativeDestruct() override;

	UFUNCTION()
	void BtnRegisterOnClicked();

	UFUNCTION()
	void BtnCancelOnClicked();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void OnSuccessRegister();

	UFUNCTION()
	void ToPreviousMenu();

protected:
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* OpacityCurveFloat = nullptr;
	
	UPROPERTY()
	FTimeline OpacityTimeLine;

	UPROPERTY()
	FOnTimelineFloat OpacityTimelineFloat;
	
	UPROPERTY(EditDefaultsOnly)
	class UMediaPlayer* MediaPlayerPreviewCard = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	class UMediaSource* MediaSourcePreviewCard = nullptr;

public:
	UPROPERTY(BlueprintReadOnly)
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* FirstName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* LastName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* Email = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* Password = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UZSpaceButton* BtnRegister = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UButton* BtnCancel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UCanvasPanel* CentralPart = nullptr;

protected:
	UFUNCTION()
	void UpdatingOpacityTimeLine(float Value);
	
	void BindOnTextCommittedEvent();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	virtual EWidgetType GetWidgetType_Implementation() override;
};
