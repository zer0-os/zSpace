// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"
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

	UFUNCTION()
	void BtnRegisterOnClicked();

	UFUNCTION()
	void BtnCancelOnClicked();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void OnSuccessRegister(class UResolutionAndWidgetDataAsset* LoginDataAsset);

public:
	UPROPERTY(BlueprintReadOnly)
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* FirstName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* LastName = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* Email = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* Password = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* BtnRegister = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* BtnCancel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UResolutionAndWidgetDataAsset* PreLoginDataAsset = nullptr;
};
