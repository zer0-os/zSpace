// Copyright 2020 Sabre Dart Studios


#include "VRRegUserWidget.h"

#include "VRLoginAndRegWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UVRRegUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
	
}

void UVRRegUserWidget::NativePreConstruct()
{
	if(IsValid(RegisterButton))
	{
		RegisterButton->OnClicked.AddUniqueDynamic(this, &UVRRegUserWidget::OnClickRegisterButton);
	}
	if(IsValid(LoginOrRegWidgetButton))
	{
		LoginOrRegWidgetButton->OnClicked.AddUniqueDynamic(this, &UVRRegUserWidget::OnClickLoginOrRegWidgetButton);
	}
	if(IsValid(EditableTextBoxFirstName))
	{
		EditableTextBoxFirstName->SetHintText(FText::FromString("First Name"));
	}
	if(IsValid(EditableTextBoxEmail))
	{
		EditableTextBoxEmail->SetHintText(FText::FromString("Email-ID"));
	}
	if(IsValid(EditableTextBoxLastName))
	{
		EditableTextBoxLastName->SetHintText(FText::FromString("Last Name"));
	}
	if(IsValid(EditableTextBoxPassword))
	{
		EditableTextBoxPassword->SetHintText(FText::FromString("Password"));
	}
	Super::NativePreConstruct();	
}

void UVRRegUserWidget::OnClickRegisterButton()
{
	checkf(nullptr != EditableTextBoxFirstName, TEXT("The EditableTextBoxFirstName is nullptr."));
	checkf(nullptr != EditableTextBoxLastName, TEXT("The EditableTextBoxLastName is nullptr."));
	checkf(nullptr != EditableTextBoxEmail, TEXT("The EditableTextBoxEmail is nullptr."));
	checkf(nullptr != EditableTextBoxPassword, TEXT("The EditableTextBoxPassword is nullptr."));
	const FString L_FirstName = EditableTextBoxFirstName->GetText().ToString();
	const FString L_LastName = EditableTextBoxLastName->GetText().ToString();
	const FString L_EmailID = EditableTextBoxEmail->GetText().ToString();
	const FString L_Password = EditableTextBoxPassword->GetText().ToString();
	checkf(nullptr != VRLoginAndRegWidget, TEXT("The VRLoginAndRegWidget is nullptr."));
	if(IsValid(VRLoginAndRegWidget))
	{
		VRLoginAndRegWidget->Reg(L_EmailID, L_Password, L_FirstName, L_LastName);
	}
}

void UVRRegUserWidget::OnClickLoginOrRegWidgetButton()
{
	checkf( nullptr != VRLoginAndRegWidget, TEXT("The VRLoginAndRegWidget is nullptr."));
	if(IsValid(VRLoginAndRegWidget))
	{
		VRLoginAndRegWidget->SwitchLoginOrRegUserWidget();
	}
}
