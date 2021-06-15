// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRRegUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UVRRegUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxFirstName = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxLastName = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxEmail = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UEditableTextBox * EditableTextBoxPassword = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true,BindWidgetOptional))
	class UButton * RegisterButton = nullptr;

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
	void OnClickRegisterButton();

	UFUNCTION()
	void OnClickLoginOrRegWidgetButton();
};
