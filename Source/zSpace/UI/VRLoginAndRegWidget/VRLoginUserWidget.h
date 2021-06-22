// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "VRLoginAndRegWidget.h"
#include "Blueprint/UserWidget.h"
#include "VRLoginUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ZSPACE_API UVRLoginUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxLogin = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxPassword = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UButton * LoginButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UButton * LoginOrRegWidgetButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UVRLoginAndRegWidget * VRLoginAndRegWidget  = nullptr;

	friend class UVRLoginAndRegWidget;

protected:
	
	virtual void NativeConstruct() override;


	virtual void NativePreConstruct() override;
	
public:

	UFUNCTION()
	void OnLoginButtonClick();

	
	UFUNCTION()
	void OnClickLoginOrRegWidgetButton();
};
