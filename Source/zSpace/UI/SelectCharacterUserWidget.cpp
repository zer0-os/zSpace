// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterUserWidget.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include <MediaAssets/Public/MediaSource.h>
#include <MediaAssets/Public/MediaPlayer.h>
#include "SelectCharacterBoxUserWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Game/ZSpaceGameInstance.h"
#include <Components/BorderSlot.h>
#include "Components/Border.h"
#include "../Types/UITypes.h"
#include "Components/WidgetSwitcher.h"

void USelectCharacterUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(MediaPlayer) && IsValid(MediaSource))
	{
		MediaPlayer->OpenSource(MediaSource);
	}

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		if (!PlayerController->OnEscButtonPressed.IsAlreadyBound(this, &USelectCharacterUserWidget::ToPreviousMenu))
		{
			PlayerController->OnEscButtonPressed.AddDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
		}
	}
}

void USelectCharacterUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
	}
}

void USelectCharacterUserWidget::ToPreviousMenu()
{
	if(IsValid(CreateNewCharacterWidget) && CreateNewCharacterWidget->IsInViewport())
	{
		CreateNewCharacterWidget->RemoveFromParent();
		return;
	}
	
	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (!IsValid(GameInstance)) return;

	UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	if (!IsValid(ManageWidgetsResolution)) return;

	const EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UResolutionAndWidgetDataAsset* WidgetDataAsset = IUIResolutionInterface::Execute_GetToPreviousMenuDataAsset(this);
	const TSubclassOf<UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);

	AZSPlayerController* PlayerController = Cast<AZSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
	}

	RemoveFromParent();
}

void USelectCharacterUserWidget::ShowCreateNewCharacterWidget(TSubclassOf<UUserWidget> Class)
{
	if (IsValid(CreateNewCharacterWidget))
	{
		CreateNewCharacterWidget->AddToViewport();
	}
	else
	{
		CreateNewCharacterWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), Class);
		if (IsValid(CreateNewCharacterWidget))
		{
			CreateNewCharacterWidget->AddToViewport();
		}
	}
}

void USelectCharacterUserWidget::HideCreateNewCharacterWidget()
{
	if (IsValid(CreateNewCharacterWidget))
	{
		CreateNewCharacterWidget->RemoveFromParent();
	}
}

void USelectCharacterUserWidget::CreateCharacterSelectBox(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo, UBorder* ParentBorder)
{
	if (!IsValid(ParentBorder)) return;
	check(SelectCharacterBoxSubClass);

	USelectCharacterBoxUserWidget* CharacterBox = Cast<USelectCharacterBoxUserWidget>(ParentBorder->GetChildAt(0));
	if (IsValid(CharacterBox))
	{
		CharacterBox->SetupWidget(CharacterSelectBoxInfo);
		return;
	}

	auto* NewWidget = CreateWidget<USelectCharacterBoxUserWidget>(GetOwningPlayer(), SelectCharacterBoxSubClass);
	if (IsValid(NewWidget))
	{
		UPanelSlot* PanelSlot = ParentBorder->AddChild(NewWidget);
		UBorderSlot* BorderPanelSlot = Cast<UBorderSlot>(PanelSlot);
		if (IsValid(BorderPanelSlot))
		{
			BorderPanelSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			BorderPanelSlot->SetHorizontalAlignment(HAlign_Fill);
			BorderPanelSlot->SetVerticalAlignment(VAlign_Fill);

			NewWidget->SetupWidget(CharacterSelectBoxInfo);
		}
	}
}

void USelectCharacterUserWidget::ShowCharacters(const TArray<FUserCharacter>& UserCharacters, const int32 CurrentCharacterIndex)
{
	auto CheckAndCreate = [this, UserCharacters](const int32 CheckIndex, UBorder* Border) -> void
	{
		if (UserCharacters.IsValidIndex(CheckIndex))
		{
			FCharacterSelectBoxInfo	CharacterInfo;	
			const FUserCharacter& UserCharacter = UserCharacters[CheckIndex];
			CharacterInfo.CharacterName = UserCharacter.CharacterName;
			CharacterInfo.CharacterLevel = FString::FromInt(UserCharacter.Level);
			CreateCharacterSelectBox(CharacterInfo, Border);
			auto* Child = Cast<USelectCharacterBoxUserWidget>(Border->GetChildAt(0));
			if (IsValid(Child))
			{
				const bool bIsShowButtons =  Border == SelectCharacterMiddleCanvas;
				Child->WidgetSwitcherEditMode->SetVisibility(bIsShowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				Child->WidgetSwitcherDoneEditMode->SetVisibility(bIsShowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			}
			
		}
		else
		{
			auto* Child = Border->GetChildAt(0);
			if (IsValid(Child))
			{
				Child->RemoveFromParent();
			}
		}
	};

	CheckAndCreate(CurrentCharacterIndex, SelectCharacterMiddleCanvas);
	CheckAndCreate(CurrentCharacterIndex - 1, SelectCharacterLeftCanvas);
	CheckAndCreate(CurrentCharacterIndex + 1, SelectCharacterRightCanvas);
}
