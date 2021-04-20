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
