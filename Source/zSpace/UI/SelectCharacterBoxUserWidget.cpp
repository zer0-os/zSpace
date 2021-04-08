// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterBoxUserWidget.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>

void USelectCharacterBoxUserWidget::SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo)
{
	PlayerName->SetText(FText::FromString(CharacterSelectBoxInfo.PlayerName));
	PlayerLevel->SetText(FText::FromString(CharacterSelectBoxInfo.PlayerLevel));
}
