// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/RegisterUserWidgetBase.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "Components/EditableTextBox.h"
#include "../Game/ZSpaceGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/UI/ZSpaceButton.h"
#include "LoginUserWidgetBase.h"
#include "Components/Button.h"
#include "../Types/UITypes.h"

void URegisterUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(BtnRegister))
	{
		UButton* Button = BtnRegister->ButtonWidget;
		if (IsValid(Button))
		{
			if (!Button->OnClicked.IsAlreadyBound(this, &URegisterUserWidgetBase::BtnRegisterOnClicked))
			{
				Button->OnClicked.AddDynamic(this, &URegisterUserWidgetBase::BtnRegisterOnClicked);
			}
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

	BindOnTextCommittedEvent();

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		if (!PlayerController->OnEscButtonPressed.IsAlreadyBound(this, &URegisterUserWidgetBase::ToPreviousMenu))
		{
			PlayerController->OnEscButtonPressed.AddDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
		}
	}
}

void URegisterUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
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
	
	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
}

void URegisterUserWidgetBase::OnSuccessRegister(UResolutionAndWidgetDataAsset* LoginDataAsset)
{
	if (!IsValid(ManageWidgetsResolution)) return;

	TSubclassOf<class UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, LoginDataAsset);
	EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
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

void URegisterUserWidgetBase::ToPreviousMenu()
{
	EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UResolutionAndWidgetDataAsset* WidgetDataAsset = IUIResolutionInterface::Execute_GetToPreviousMenuDataAsset(this);
	TSubclassOf<UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
	}

	RemoveFromParent();
}

void URegisterUserWidgetBase::BindOnTextCommittedEvent()
{
	auto Bind = [this] (UEditableTextBox* EditableTextBox) -> void
	{
		if (IsValid(EditableTextBox) && !EditableTextBox->OnTextCommitted.IsAlreadyBound(this, &URegisterUserWidgetBase::OnTextCommitted))
		{
			EditableTextBox->OnTextCommitted.AddDynamic(this, &URegisterUserWidgetBase::OnTextCommitted);
		}
	};

 	Bind(FirstName);
 	Bind(LastName);
 	Bind(Email);
 	Bind(Password);
}

void URegisterUserWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		BtnRegisterOnClicked();
	}
}

