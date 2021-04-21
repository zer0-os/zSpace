// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterBoxUserWidget.h"
#include <Components/WidgetSwitcher.h>
#include "zSpace/UI/ZSpaceButton.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/Image.h>

void USelectCharacterBoxUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(EditModeButton))
	{
		if (!EditModeButton->OnClicked.IsAlreadyBound(this, &USelectCharacterBoxUserWidget::OnClickedEditModeButton))
		{
			EditModeButton->OnClicked.AddDynamic(this, &USelectCharacterBoxUserWidget::OnClickedEditModeButton);
		}
	}
	if (IsValid(NextCharacterMesh))
	{
		NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
		NextCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh);
	}
	if (IsValid(PreviousCharacterMesh))
	{
		PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
		PreviousCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh);
	}
}

void USelectCharacterBoxUserWidget::SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo)
{
	PlayerName->SetText(FText::FromString(CharacterSelectBoxInfo.CharacterName));
	PlayerLevel->SetText(FText::FromString(CharacterSelectBoxInfo.CharacterLevel));
}

void USelectCharacterBoxUserWidget::OnClickedEditModeButton()
{
	bIsEditMode = true;
	bIsCreateCharacterMode = false;
	
	WidgetSwitcherEditMode->SetActiveWidget(RemoveCharacterButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(DoneEditModeButton);

	NextCharacterMesh->SetVisibility(ESlateVisibility::Visible);
	PreviousCharacterMesh->SetVisibility(ESlateVisibility::Visible);
}

void USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh()
{
	
}

void USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh()
{
	
}

void USelectCharacterBoxUserWidget::ChangeCreateCharacterMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = true;
	
	WidgetSwitcherEditMode->SetActiveWidget(RemoveCharacterButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(CreateCharacterDone);

	CreateCharacterNameSwitcher->SetActiveWidget(NewCharacterName);

	// TODO Fix Level 
	PlayerLevel->SetText(FText::FromString("LEVEL 0"));
}

void USelectCharacterBoxUserWidget::ChangeNormalMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = false;
	
	WidgetSwitcherEditMode->SetActiveWidget(EditModeButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(SelectButton);
	CreateCharacterNameSwitcher->SetActiveWidget(PlayerName);

	NewCharacterName->SetText(FText::FromString(""));
}
