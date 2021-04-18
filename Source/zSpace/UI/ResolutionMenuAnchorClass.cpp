// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/ResolutionMenuAnchorClass.h"

#include "Components/Button.h"

UButton* UResolutionMenuAnchorClass::CreateButton(const FButtonStyle ButtonStyle, EResolution Resolution)
{
	UButton* Button = NewObject<UButton>(this);
	if (IsValid(Button))
	{
		Button->SetStyle(ButtonStyle);
		ResolutionAndButton.Add(Resolution, Button);	
	}

	return Button;
}
