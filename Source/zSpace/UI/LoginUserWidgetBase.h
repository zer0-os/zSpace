// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"
#include "../Interfaces/UIResolutionInterface.h"
#include "Components/TimelineComponent.h"

#include "LoginUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API ULoginUserWidgetBase : public UOWSLoginWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Login();

	UFUNCTION()
	void OnClickedLoginEnter();
	
	UFUNCTION()
	void OnClickedBtnPreviousMenu();

public:
	UPROPERTY(EditDefaultsOnly)
	class UMediaPlayer* MediaPlayerPreviewCard = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	class UMediaSource* MediaSourcePreviewCard = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* txtEmail = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* txtPassword = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UCanvasPanel* CentralPart = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UZSpaceButton* LoginEnter = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UButton* BtnPreviousMenu = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* OpacityCurveFloat = nullptr;
	
	UPROPERTY()
	FTimeline OpacityTimeLine;

	UPROPERTY()
	FOnTimelineFloat OpacityTimelineFloat;

protected:
	virtual EWidgetType GetWidgetType_Implementation() override;
	
	UFUNCTION()
	void UpdatingOpacityTimeLine(float Value);
	
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable)
	void UnBindToPreviousMenuEvent();

	UFUNCTION()
	void ToPreviousMenu();
};
