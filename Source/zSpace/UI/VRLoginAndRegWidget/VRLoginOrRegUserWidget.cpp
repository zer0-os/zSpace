// Copyright 2020 Sabre Dart Studios


#include "VRLoginOrRegUserWidget.h"

#include "VRLoginAndRegWidget.h"
#include "Components/Button.h"

void UVRLoginOrRegUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(IsValid(ButtonLogin))
	{
		ButtonLogin->OnClicked.AddUniqueDynamic(this, &UVRLoginOrRegUserWidget::OnClickButtonLogin);
	}
	if(IsValid(ButtonReg))
	{
		ButtonReg->OnClicked.AddUniqueDynamic(this, &UVRLoginOrRegUserWidget::OnClickButtonReg);
	}
		
}

void UVRLoginOrRegUserWidget::OnClickButtonLogin()
{
	checkf(nullptr != VRLoginAndRegWidget, TEXT("The VRLoginAndRegWidget is nullptr."));
	if(VRLoginAndRegWidget)
	{
		VRLoginAndRegWidget->SwitchLoginWidget();
	}
}

void UVRLoginOrRegUserWidget::OnClickButtonReg()
{
	checkf(nullptr != VRLoginAndRegWidget, TEXT("The VRLoginAndRegWidget is nullptr."));
	if(VRLoginAndRegWidget)
	{
		VRLoginAndRegWidget->SwitchRegWidget();
	}
}
