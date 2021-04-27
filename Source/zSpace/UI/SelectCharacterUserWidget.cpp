// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterUserWidget.h"

#include "ZSCustomButton.h"
#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "zSpace/UI/SelectCharacterBoxUserWidget.h"
#include "../PlayerController/ZSLoginPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "../Game/ZSpaceGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "Components/BorderSlot.h"
#include "Components/Border.h"
#include "../Types/UITypes.h"
#include "Kismet/KismetSystemLibrary.h"

void USelectCharacterUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.AddUniqueDynamic(this, &USelectCharacterUserWidget::ToPreviousMenu);
	}

	const bool bIsValidBorders = IsValid(SelectCharacterMiddleCanvas) && IsValid(SelectCharacterRightCanvas) && IsValid(SelectCharacterLeftCanvas);
	if (bIsValidBorders)
	{
		MainCharacterBox = SelectCharacterMiddleCanvas;
		RightCharacterBox = SelectCharacterRightCanvas;
		LeftCharacterBox = SelectCharacterLeftCanvas;
	}
}

void USelectCharacterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectCharacterUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
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

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
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
		if (!IsValid(Border)) return;
		
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
				const bool bIsShowButtons =  Border == MainCharacterBox;
				Child->WidgetSwitcherEditMode->SetVisibility(bIsShowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				Child->WidgetSwitcherDoneEditMode->SetVisibility(bIsShowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				if (!bIsShowButtons)
				{
					Child->NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
					Child->PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
				}
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

	CheckAndCreate(CurrentCharacterIndex, MainCharacterBox);

	// Right
	const bool bIsRightBordersEqual = RightCharacterBox == SelectCharacterRightCanvas;
	int8 Value = bIsRightBordersEqual ? 1 : -1;
	CheckAndCreate(CurrentCharacterIndex + Value, RightCharacterBox);

	// Left
	const bool bIsLeftBordersEqual = LeftCharacterBox == SelectCharacterLeftCanvas;
	Value = bIsLeftBordersEqual ? -1 : 1;
	CheckAndCreate(CurrentCharacterIndex + Value, LeftCharacterBox);
	
	// UKismetSystemLibrary::PrintString(this, FString::FromInt(CurrentCharacterIndex));
}

USelectCharacterBoxUserWidget* USelectCharacterUserWidget::GetSelectedCharacterBox() const
{
	if (IsValid(MainCharacterBox))
	{
		auto* Child = MainCharacterBox->GetChildAt(0);

		return Cast<USelectCharacterBoxUserWidget>(Child);
	}

	return nullptr;
}

void USelectCharacterUserWidget::SetMainCharacterBox(UBorder* NewValue)
{
	MainCharacterBox = NewValue;
}

void USelectCharacterUserWidget::UpdateBorderToRight()
{
	LeftCharacterBox = SelectCharacterRightCanvas;
	MainCharacterBox = SelectCharacterLeftCanvas;
	RightCharacterBox = SelectCharacterMiddleCanvas;
}

void USelectCharacterUserWidget::UpdateBorderToLeft()
{
	MainCharacterBox = SelectCharacterRightCanvas;
	RightCharacterBox = SelectCharacterLeftCanvas;
	LeftCharacterBox = SelectCharacterMiddleCanvas;
}
