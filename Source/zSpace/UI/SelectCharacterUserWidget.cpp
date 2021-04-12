// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterUserWidget.h"

#include <MediaAssets/Public/MediaSource.h>
#include <MediaAssets/Public/MediaPlayer.h>
#include "SelectCharacterBoxUserWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/BorderSlot.h>
#include "Components/Border.h"
#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../Types/UITypes.h"
#include "../PlayerController/ZPPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "../Game/ZSpaceGameInstance.h"

EWidgetType USelectCharacterUserWidget::GetWidgetType_Implementation()
{
	return EWidgetType::SelectCharacter;
}

void USelectCharacterUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(MediaPlayer) && IsValid(MediaSource))
	{
		MediaPlayer->OpenSource(MediaSource);
	}

	AZPPlayerController* PlayerController = Cast<AZPPlayerController>(GetOwningPlayer());
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

	AZPPlayerController* PlayerController = Cast<AZPPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
	}
}

void USelectCharacterUserWidget::ToPreviousMenu()
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

	AZPPlayerController* PlayerController = Cast<AZPPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
	}

	RemoveFromParent();
}

void USelectCharacterUserWidget::CreateCharacterSelectBox(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo)
{
	check(SelectCharacterBoxSubClass);

	if (IsValid(SelectedCharacterBoxUserWidget))
	{
		SelectedCharacterBoxUserWidget->SetupWidget(CharacterSelectBoxInfo);
		return;
	}

	SelectedCharacterBoxUserWidget = CreateWidget<USelectCharacterBoxUserWidget>(GetOwningPlayer(), SelectCharacterBoxSubClass);
	if (IsValid(SelectedCharacterBoxUserWidget))
	{
		UPanelSlot* PanelSlot = SelectCharacterMiddleCanvas->AddChild(SelectedCharacterBoxUserWidget);
		UBorderSlot* BorderPanelSlot = Cast<UBorderSlot>(PanelSlot);
		if (IsValid(BorderPanelSlot))
		{
			BorderPanelSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			BorderPanelSlot->SetHorizontalAlignment(HAlign_Fill);
			BorderPanelSlot->SetVerticalAlignment(VAlign_Fill);

			SelectedCharacterBoxUserWidget->SetupWidget(CharacterSelectBoxInfo);
		}
	}
}

void USelectCharacterUserWidget::ShowCharacters(TArray<FUserCharacter>& UserCharacters, int32 CurrentCharacterIndex)
{
	if (UserCharacters.IsValidIndex(CurrentCharacterIndex))
	{
		FUserCharacter& CurrentSelectedCharacter = UserCharacters[CurrentCharacterIndex];

		if (UserCharacters.Num() >= 1)
		{
			for (FUserCharacter& User : UserCharacters)
			{
				if (User.CharacterName != CurrentSelectedCharacter.CharacterName)
				{
					if (SelectCharacterLeftCanvas->GetAllChildren().Num() == 0)
					{
						// CreateWidget<>()

					}
					else if (SelectCharacterRightCanvas->GetAllChildren().Num() == 0)
					{

					}
				}
			}
		}
	}
}
