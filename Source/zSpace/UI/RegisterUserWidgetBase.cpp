// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/RegisterUserWidgetBase.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../Components/ManageWidgetsResolution.h"
#include <Components/EditableTextBox.h>
#include "../Game/ZSpaceGameInstance.h"
#include <Components/Button.h>
#include "../Types/UITypes.h"
#include "LoginUserWidgetBase.h"
#include <Kismet/KismetSystemLibrary.h>

void URegisterUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(BtnRegister))
	{
		if (!BtnRegister->OnClicked.IsAlreadyBound(this, &URegisterUserWidgetBase::BtnRegisterOnClicked))
		{
			BtnRegister->OnClicked.AddDynamic(this, &URegisterUserWidgetBase::BtnRegisterOnClicked);
		}
	}
	if (IsValid(BtnCancel))
	{
		if (!BtnCancel->OnClicked.IsAlreadyBound(this, &URegisterUserWidgetBase::BtnCancelOnClicked))
		{
			BtnCancel->OnClicked.AddDynamic(this, &URegisterUserWidgetBase::BtnCancelOnClicked);
		}
	}

	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	}
}

void URegisterUserWidgetBase::BtnRegisterOnClicked()
{
	const FString StringFirstName = FirstName->GetText().ToString();
	const FString StringLastName = LastName->GetText().ToString();
	const FString StringEmail = Email->GetText().ToString();
	const FString StringPassword = Password->GetText().ToString();

	Register(StringEmail, StringPassword, StringFirstName, StringLastName);
}

void URegisterUserWidgetBase::BtnCancelOnClicked()
{
	if (!IsValid(ManageWidgetsResolution)) return;
	
	RemoveFromParent();
	
	TSubclassOf<class UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, PreLoginDataAsset);
	EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UUserWidget* Widget = nullptr;
	
	ManageWidgetsResolution->CreateWidgetAndAddViewprot(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
}

void URegisterUserWidgetBase::OnSuccessRegister(UResolutionAndWidgetDataAsset* LoginDataAsset)
{
	if (!IsValid(ManageWidgetsResolution)) return;

	TSubclassOf<class UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, LoginDataAsset);
	EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewprot(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
	if (IsValid(Widget))
	{
		ULoginUserWidgetBase* LoginWidget = Cast<ULoginUserWidgetBase>(Widget);
		if (IsValid(LoginWidget))
		{
			LoginWidget->txtEmail->SetText(Email->GetText());
		}
	}

	RemoveFromParent();
}

