// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterUserWidget.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSLoginPlayerController.h"
#include "zSpace/UI/SelectCharacterBoxUserWidget.h"
#include "../Components/ManageWidgetsResolution.h"
#include "zSpace/Actors/PreviewCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Game/ZSpaceGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "zSpace/UI/ZSCustomButton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BorderSlot.h"
#include "Components/Border.h"
#include "../Types/UITypes.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/Image.h>

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

	if (IsValid(AddNewCharacter))
	{
		AddNewCharacter->OnClicked.AddUniqueDynamic(this, &USelectCharacterUserWidget::OnClickedAddNewCharacter);	
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

	ResetBoxesTransform();
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

void USelectCharacterUserWidget::CreateCharacterSelectBox(const FCharacterInfoForUI& CharacterSelectBoxInfo, UBorder* ParentBorder)
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
		NewWidget->SelectCharacterUserWidget = this;
		
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
	auto CheckAndCreate = [this, UserCharacters](const int32 CheckIndex, UBorder* Border) -> USelectCharacterBoxUserWidget*
	{
		if (!IsValid(Border)) return nullptr;
		
		if (UserCharacters.IsValidIndex(CheckIndex))
		{
			FCharacterInfoForUI	CharacterInfo;	
			const FUserCharacter& UserCharacter = UserCharacters[CheckIndex];
			CharacterInfo.CharacterName = UserCharacter.CharacterName;
			CharacterInfo.CharacterLevel = FString::FromInt(UserCharacter.Level);

			CreateCharacterSelectBox(CharacterInfo, Border);

			USelectCharacterBoxUserWidget*  L_SelectCharacterMiddleBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterMiddleBorder->GetChildAt(0));
			if (L_SelectCharacterMiddleBox)
			{
				L_SelectCharacterMiddleBox->CreateCharacterNameSwitcher->SetRenderScale(FVector2D(1.3f, 1.3f));
			}

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
				SetPreviewCharacterPositionByCharacterBox(Child);
				return Child;
			}
		}
		else
		{
			auto* Child = Cast<USelectCharacterBoxUserWidget>(Border->GetChildAt(0));
			if (IsValid(Child))
			{
				Child->RemoveFromParent();
				return Child;
			}
		}

		return nullptr;
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

	if (UserCharacters.Num() == 1)
	{
		CheckAndCreate(CurrentCharacterIndex, MainCharacterBox);
		LeftCharacterBox->RemoveChildAt(0);
		RightCharacterBox->RemoveChildAt(0);
		AnimationBorderLeft->RemoveChildAt(0);
		
		return;
	}
	else
	{
		CheckAndCreate(CurrentCharacterIndex, MainCharacterBox);
	}

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

TArray<FCharacterInfoForUI> USelectCharacterUserWidget::GetCharactersInfoData()
{	
	TArray<FCharacterInfoForUI> Result;

	auto AddData = [this, &Result](UBorder* Border) -> void
	{
		const auto Data = GetCharacterInfoForUI(Border);
		if (Data.Key)
		{
			Result.AddUnique(Data.Value);
		}
	};
	
	AddData(LeftCharacterBox);
	AddData(MainCharacterBox);
	AddData(RightCharacterBox);
	
	return Result;
}

APreviewCharacter* USelectCharacterUserWidget::GetPreviewCharacterByEnum(
	EPreviewCharacterPosition P_PreviewCharacterPosition) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APreviewCharacter::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		APreviewCharacter* L_PreviewCharacter = Cast<APreviewCharacter>(Actor);
		if (IsValid(L_PreviewCharacter) && L_PreviewCharacter->PreviewCharacterPosition == P_PreviewCharacterPosition)
		{
			return L_PreviewCharacter;
		}
	}

	return nullptr;	
}

bool USelectCharacterUserWidget::CanChangeCharacter()
{
	auto* Widget = GetSelectedCharacterBox();
	if (IsValid(Widget))
	{
		const bool Result = Widget->bIsEditMode || Widget->bIsCreateCharacterMode;
		if (Result)
		{
			return false;
		}
		else
		{
			return !(GetCharactersInfoData().Num() == 1);
		}
	}

	return false;
}

class UTexture2D* USelectCharacterUserWidget::GetBackGroundImage()
{
	int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, BackGroundImages.Num() - 1);
	if (BackGroundImages.IsValidIndex(Index))
	{
		return BackGroundImages[Index];
	}
	return nullptr;
}

void USelectCharacterUserWidget::UpdateBackgroundImage(bool bisRight)
{
	USelectCharacterBoxUserWidget* AnimationBox = Cast<USelectCharacterBoxUserWidget>(AnimationBorderLeft->GetChildAt(0));
	USelectCharacterBoxUserWidget* LeftBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterLeftBorder->GetChildAt(0));
	USelectCharacterBoxUserWidget* MiddleBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterMiddleBorder->GetChildAt(0));
	USelectCharacterBoxUserWidget* RightBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterRightBorder->GetChildAt(0));
	
	if (IsValid(AnimationBox) && IsValid(LeftBox) &&IsValid(MiddleBox) && IsValid(RightBox))
	{
		if (bisRight)
		{	
			if (LastChangeCharacterDirection == EChangeCharacterDirection::ToRight)
			{
				RightBox->BackgroundImage->Brush.SetResourceObject(MiddleBox->BackgroundImage->Brush.GetResourceObject());
				MiddleBox->BackgroundImage->Brush.SetResourceObject(LeftBox->BackgroundImage->Brush.GetResourceObject());
				LeftBox->BackgroundImage->Brush.SetResourceObject(AnimationBox->BackgroundImage->Brush.GetResourceObject());
				AnimationBox->BackgroundImage->Brush.SetResourceObject(GetBackGroundImage());		
			}
		}
		else
		{
			if(LastChangeCharacterDirection == EChangeCharacterDirection::ToLeft)
			{
				AnimationBox->BackgroundImage->Brush.SetResourceObject(LeftBox->BackgroundImage->Brush.GetResourceObject());
				LeftBox->BackgroundImage->Brush.SetResourceObject(MiddleBox->BackgroundImage->Brush.GetResourceObject());
				MiddleBox->BackgroundImage->Brush.SetResourceObject(RightBox->BackgroundImage->Brush.GetResourceObject());
				RightBox->BackgroundImage->Brush.SetResourceObject(GetBackGroundImage());
			}
		}
	}
}

void USelectCharacterUserWidget::UpdateBorderToRight()
{
	USelectCharacterBoxUserWidget* RightSelectBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterRightBorder->GetChildAt(0));
	if (RightSelectBox)
	{
		RightSelectBox->SetupWidget(GetCharacterInfoForUI(RightCharacterBox).Value);
	}

	LeftCharacterBox = AnimationBorderLeft;
	MainCharacterBox = SelectCharacterLeftBorder;
	RightCharacterBox = SelectCharacterMiddleBorder;

	USelectCharacterBoxUserWidget* SelectCharacterLeftBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterLeftBorder->GetChildAt(0));
	if (SelectCharacterLeftBox)
	{
		SelectCharacterLeftBox->PlayButtonsFadeInAnimation();
	}

	UpdateBackgroundImage(true);

	LastChangeCharacterDirection = EChangeCharacterDirection::ToRight;
}

void USelectCharacterUserWidget::UpdateBorderToLeft()
{
	USelectCharacterBoxUserWidget* AnimationSelectBox = Cast<USelectCharacterBoxUserWidget>(AnimationBorderLeft->GetChildAt(0));
	if (AnimationSelectBox)
	{
		AnimationSelectBox->SetupWidget(GetCharacterInfoForUI(LeftCharacterBox).Value);
	}

	MainCharacterBox = SelectCharacterMiddleBorder;
	RightCharacterBox = SelectCharacterRightBorder;
	LeftCharacterBox = SelectCharacterLeftBorder;

	USelectCharacterBoxUserWidget* SelectCharacterMiddleBox = Cast<USelectCharacterBoxUserWidget>(SelectCharacterMiddleBorder->GetChildAt(0));
	if (SelectCharacterMiddleBox)
	{
		SelectCharacterMiddleBox->PlayButtonsFadeInAnimation();
	}
	UpdateBackgroundImage(false);

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

void USelectCharacterUserWidget::SetPreviewCharacterPositionByCharacterBox(USelectCharacterBoxUserWidget* Widget)
{
	if (Widget == LeftCharacterBox->GetChildAt(0))
	{
		Widget->SetPreviewCharacterPosition(EPreviewCharacterPosition::L_1);
		return;
	}

	if (Widget == MainCharacterBox->GetChildAt(0))
	{
		Widget->SetPreviewCharacterPosition(EPreviewCharacterPosition::Middle);
		return;
	}

	if (Widget == RightCharacterBox->GetChildAt(0))
	{
		Widget->SetPreviewCharacterPosition(EPreviewCharacterPosition::R_1);
	}
	
	if (Widget == AnimationBorderLeft->GetChildAt(0))
	{
		Widget->SetPreviewCharacterPosition(EPreviewCharacterPosition::L_2);
	}
}

TPair<bool, FCharacterInfoForUI> USelectCharacterUserWidget::GetCharacterInfoForUI(const UBorder* Border) const
{
	if (IsValid(Border))
	{
		USelectCharacterBoxUserWidget* Child = Cast<USelectCharacterBoxUserWidget>(Border->GetChildAt(0));
		if (IsValid(Child))
		{
			return TPair<bool, FCharacterInfoForUI>(true, Child->GetCharacterInfoForUI());
		}
	}

	return TPair<bool, FCharacterInfoForUI>(false, FCharacterInfoForUI());
}

void USelectCharacterUserWidget::OnClickedAddNewCharacter()
{
	USelectCharacterBoxUserWidget* Widget = GetSelectedCharacterBox();
	if (IsValid(Widget))
	{
		Widget->ChangeCreateCharacterMode();
	}
	else
	{
		CreateCharacterSelectBox(FCharacterInfoForUI(), MainCharacterBox);
		Widget = GetSelectedCharacterBox();
		if (IsValid(Widget))
		{
			Widget->ChangeCreateCharacterMode();
			SetPreviewCharacterPositionByCharacterBox(Widget);
		}
	}
}
