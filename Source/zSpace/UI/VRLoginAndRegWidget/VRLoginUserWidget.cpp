// Copyright 2020 Sabre Dart Studios


#include "VRLoginUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UVRLoginUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UVRLoginUserWidget::NativePreConstruct()
{
	if(IsValid(LoginButton))
	{
		LoginButton->OnClicked.AddUniqueDynamic(this, &UVRLoginUserWidget::OnLoginButtonClick);
	}
	if(IsValid(LoginOrRegWidgetButton))
	{
		LoginOrRegWidgetButton->OnClicked.AddUniqueDynamic(this, &UVRLoginUserWidget::OnClickLoginOrRegWidgetButton);
	}
	if(IsValid(EditableTextBoxLogin))
	{
		EditableTextBoxLogin->SetHintText(FText::FromString("Login"));
	}
	if(IsValid(EditableTextBoxPassword))
	{
		EditableTextBoxPassword->SetHintText(FText::FromString("Password"));
		EditableTextBoxPassword->SetIsPassword(true);
	}
	
	Super::NativePreConstruct();
}

void UVRLoginUserWidget::OnLoginButtonClick()
{
	if(IsValid(EditableTextBoxLogin) && IsValid(EditableTextBoxPassword))
	{
		if(IsValid(VRLoginAndRegWidget))
		{
			VRLoginAndRegWidget->Login( EditableTextBoxLogin->GetText().ToString()
									  , EditableTextBoxPassword->GetText().ToString());
		}
	}
}

void UVRLoginUserWidget::OnClickLoginOrRegWidgetButton()
{
	checkf( nullptr != VRLoginAndRegWidget, TEXT("The VRLoginAndRegWidget is nullptr."));
	if(IsValid(VRLoginAndRegWidget))
	{
		VRLoginAndRegWidget->SwitchLoginOrRegUserWidget();
	}
}
