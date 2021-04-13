// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/LoginUserWidgetBase.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "Components/EditableTextBox.h"
#include "../Game/ZSpaceGameInstance.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Types/UITypes.h"


void ULoginUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(txtEmail) && IsValid(txtPassword))
	{
		if (!txtEmail->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtEmail->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
		if (!txtPassword->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtPassword->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
	}

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		if (!PlayerController->OnEscButtonPressed.IsAlreadyBound(this, &ULoginUserWidgetBase::ToPreviousMenu))
		{
			PlayerController->OnEscButtonPressed.AddDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
		}
	}
}

void ULoginUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
	}
}

void ULoginUserWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnPressedEnter();
	}
}

void ULoginUserWidgetBase::ToPreviousMenu()
{
	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (!IsValid(GameInstance)) return;

	UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	if (!IsValid(ManageWidgetsResolution)) return;

	EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UResolutionAndWidgetDataAsset* WidgetDataAsset = IUIResolutionInterface::Execute_GetToPreviousMenuDataAsset(this);
	TSubclassOf<UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewprot(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
	}

	RemoveFromParent();
}
