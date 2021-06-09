// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/PreLoginBaseUserWidget.h"

#include "zSpace/Components/ManageWidgetsResolution.h"
#include "zSpace/Game/ZSpaceGameInstance.h"

#include "Components/Button.h"
#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"

void UPreLoginBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
		check(ManageWidgetsResolution);
	}
	if (IsValid(SignInButton) && IsValid(RegisterButton))
	{
		SignInButton->ButtonWidget->OnClicked.AddUniqueDynamic(this, &UPreLoginBaseUserWidget::OnClickedSignIn);
		RegisterButton->ButtonWidget->OnClicked.AddUniqueDynamic(this, &UPreLoginBaseUserWidget::OnClickedRegister);
	}
}

void UPreLoginBaseUserWidget::OnClickedSignIn()
{
	if (IsValid(ManageWidgetsResolution))
	{
		if (IsValid(SignInWidgetDataAsset))
		{
			APlayerController* PC = GetOwningPlayer();
			if (!IsValid(PC)) return;

			const TSubclassOf<UUserWidget>& WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, SignInWidgetDataAsset);
			const EResolution& Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

			UUserWidget* CreatedWidget = nullptr;
			ManageWidgetsResolution->CreateWidgetAndAddViewport(PC, WidgetSubClass, Resolution, CreatedWidget);
			if (IsValid(CreatedWidget))
			{
				RemoveFromParent();
			}
		}
	}
}

void UPreLoginBaseUserWidget::OnClickedRegister()
{
	if (IsValid(ManageWidgetsResolution))
	{
		if (IsValid(RegisterWidgetDataAsset))
		{
			APlayerController* PC = GetOwningPlayer();
			if (!IsValid(PC)) return;

			const TSubclassOf<UUserWidget>& WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, RegisterWidgetDataAsset);
			const EResolution& Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

			UUserWidget* CreatedWidget = nullptr;
			ManageWidgetsResolution->CreateWidgetAndAddViewport(PC, WidgetSubClass, Resolution, CreatedWidget);
			if (IsValid(CreatedWidget))
			{
				RemoveFromParent();
			}
		}
	}
}

EWidgetType UPreLoginBaseUserWidget::GetWidgetType_Implementation()
{
	return EWidgetType::PreLogin;
}
