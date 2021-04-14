// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"
#include "../Interfaces/UIResolutionInterface.h"
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

	virtual void NativeDestruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* txtEmail = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* txtPassword = nullptr;

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void OnPressedEnter();

	UFUNCTION()
	void ToPreviousMenu();
};
