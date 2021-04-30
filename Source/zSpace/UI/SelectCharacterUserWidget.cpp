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

	const bool bIsValidBorders = IsValid(SelectCharacterMiddleBorder) && IsValid(SelectCharacterRightBorder) && IsValid(SelectCharacterLeftBorder);
	if (bIsValidBorders)
	{
		MainCharacterBox = SelectCharacterMiddleBorder;
		RightCharacterBox = SelectCharacterRightBorder;
		LeftCharacterBox = SelectCharacterLeftBorder;
	}

	UZSpaceGameInstance* GameInstance = Cast<UZSpaceGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
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

	TArray<UBorder*> Borders = GetBoxBorders();
	for (UBorder* Border : Borders)
	{
		if (!IsValid(Border)) continue;
		
		auto Children = Border->GetAllChildren();
		for (auto* Child : Children)
		{
			if (IsValid(Child))
			{
				Child->RemoveFromParent();
			}
		}
	}
}

void USelectCharacterUserWidget::ToPreviousMenu()
{
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
			UKismetSystemLibrary::PrintString(this, GetNameSafe(Border));
			auto* Child = Border->GetChildAt(0);
			if (IsValid(Child))
			{
				Child->RemoveFromParent();
			}
		}
	};

	auto GetLastOrFirstIndex = [this, &UserCharacters](const int8& Index) -> uint8
	{
		if (Index < 0)
		{
			return  UserCharacters.Num() - 1;
		}
		else if (Index > UserCharacters.Num() - 1)
		{
			return 0;
		}
		else
		{
			return Index;
		}
	};
	
	int8 Index;
	int8 Value;

	CheckAndCreate(CurrentCharacterIndex, MainCharacterBox);
	if (LastChangeCharacterDirection == EChangeCharacterDirection::None)
	{
		Index = CurrentCharacterIndex + 1;
		CheckAndCreate(GetLastOrFirstIndex(Index), RightCharacterBox);

		Index = CurrentCharacterIndex - 1;
		CheckAndCreate(GetLastOrFirstIndex(Index), LeftCharacterBox);

		return;
	}
	
	if (LastChangeCharacterDirection == EChangeCharacterDirection::ToLeft)
	{
		// Index = CurrentCharacterIndex - 1;
		// CheckAndCreate(GetLastOrFirstIndex(Index), RightCharacterBox);

		// Index = CurrentCharacterIndex + 1;
		// CheckAndCreate(GetLastOrFirstIndex(Index), LeftCharacterBox);
		
		// Right
		const bool bIsRightBordersEqual = RightCharacterBox == SelectCharacterRightBorder;
		Value = bIsRightBordersEqual ? -1 : 1;
		Index = CurrentCharacterIndex + Value;
		CheckAndCreate(GetLastOrFirstIndex(Index), RightCharacterBox);

		// Left
		const bool bIsLeftBordersEqual = LeftCharacterBox == SelectCharacterLeftBorder;
		Value = bIsLeftBordersEqual ? 1 : -1;
		Index = CurrentCharacterIndex + Value;
		CheckAndCreate(GetLastOrFirstIndex(Index), LeftCharacterBox);
	}
	else if (LastChangeCharacterDirection == EChangeCharacterDirection::ToRight)
	{
		// Right
		const bool bIsRightBordersEqual = RightCharacterBox == SelectCharacterRightBorder;
		Value = bIsRightBordersEqual ? 1 : -1;
		Index = CurrentCharacterIndex + Value;
		CheckAndCreate(GetLastOrFirstIndex(Index), RightCharacterBox);

		// Left
		const bool bIsLeftBordersEqual = LeftCharacterBox == SelectCharacterLeftBorder;
		Value = bIsLeftBordersEqual ? -1 : 1;
		Index = CurrentCharacterIndex + Value;
		CheckAndCreate(GetLastOrFirstIndex(Index), LeftCharacterBox);
	}
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
	LeftCharacterBox = AnimationBorderLeft;
	MainCharacterBox = SelectCharacterLeftBorder;
	RightCharacterBox = SelectCharacterMiddleBorder;
	
	LastChangeCharacterDirection = EChangeCharacterDirection::ToRight;
}

void USelectCharacterUserWidget::UpdateBorderToLeft()
{
	LeftCharacterBox = SelectCharacterLeftBorder;
	MainCharacterBox = SelectCharacterMiddleBorder;
	RightCharacterBox = SelectCharacterRightBorder;
	
	LastChangeCharacterDirection = EChangeCharacterDirection::ToLeft;
}

TArray<UBorder*> USelectCharacterUserWidget::GetBoxBorders() const
{
	TArray<UBorder*> Result;

	Result.Add(SelectCharacterMiddleBorder);
	Result.Add(SelectCharacterRightBorder);
	Result.Add(SelectCharacterLeftBorder);
	Result.Add(AnimationBorderLeft);

	return Result;
}

void USelectCharacterUserWidget::PlayAnimationChangeCharacter(UWidgetAnimation* ChangeAnimation,
	EChangeCharacterDirection AnimationDirection)
{
	EUMGSequencePlayMode::Type UMGSequencePlayMode = EUMGSequencePlayMode::Forward;
	
	switch (AnimationDirection)
	{
	case EChangeCharacterDirection::ToRight: UMGSequencePlayMode = EUMGSequencePlayMode::Forward; break;
	case EChangeCharacterDirection::ToLeft: UMGSequencePlayMode = EUMGSequencePlayMode::Reverse; break;
	default: ;
	}

	PlayAnimation(ChangeAnimation, 0.f, 1, UMGSequencePlayMode, 1.f, false);
}

void USelectCharacterUserWidget::ResetBoxesTransform()
{
	TArray<UBorder*> Boxes = GetBoxBorders();
	for (UBorder* Border : Boxes)
	{
		if (IsValid(Border))
		{
			Border->SetRenderTransform(FWidgetTransform());
		}
	}
	
	MainCharacterBox = SelectCharacterMiddleBorder;
	RightCharacterBox = SelectCharacterRightBorder;
	LeftCharacterBox = SelectCharacterLeftBorder;

	LastChangeCharacterDirection = EChangeCharacterDirection::None;
}
