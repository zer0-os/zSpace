// Copyright 2020 Sabre Dart Studios


#include "zSpace/VirtualkeyboarActor/WIdgets/ZSKeyboardKeyUserWidget/ZSKeyboardKeyUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetInteractionComponent.h"
#include "zSpace/VirtualkeyboarActor/VirtualKeyboardActor.h"

void UZSKeyboardKeyUserWidget::SetWidgetInteractionComponent_Implementation( UWidgetInteractionComponent* NewWidgetInteractionComponent)
{
	WidgetInteractionComponent = NewWidgetInteractionComponent;
}


void UZSKeyboardKeyUserWidget::SetVirtualKeyboardActor_Implementation(AVirtualKeyboardActor* NewVirtualKeyboardActor)
{
	VirtualKeyboardActor = NewVirtualKeyboardActor;
	if(IsValid(VirtualKeyboardActor))
	{
		VirtualKeyboardActor->OnPressedShift.AddUniqueDynamic(this, &UZSKeyboardKeyUserWidget::PressedShift);
	}
}

void UZSKeyboardKeyUserWidget::NativePreConstruct()
{
	checkf(nullptr != KeyButton, TEXT("The KeyButton is nullptr."));
	if(IsValid(KeyButton))
	{
		KeyButton->OnClicked.AddUniqueDynamic(this, &UZSKeyboardKeyUserWidget::PressKey);
	}
	SetKeyText();	
	Super::NativePreConstruct();
}

void UZSKeyboardKeyUserWidget::SetKeyText()
{
	if(IsValid(TextBlockKeyName))
	{
		FString L_Key = "Error";
		if(ESpecialKey::SPECIAL == Key.IsSpecialKey)
		{
			L_Key = Key.SpecialKey.GetDisplayName().ToString();
		}
		else if(ESpecialKey::NORMAL == Key.IsSpecialKey)
		{
			if(IsValid(VirtualKeyboardActor))
			{
				if(VirtualKeyboardActor->bIsPressedShift)
				{
					L_Key = Key.ShiftPressedKey;
				}
				else
				{
					L_Key = Key.Key;
				}
			}
			else
			{
				L_Key = Key.Key;
			}
		}
		TextBlockKeyName->SetText(FText::FromString(L_Key));
	}
}

void UZSKeyboardKeyUserWidget::PressedShift(const bool NewIsPressedShift)
{
	SetKeyText();	
}

void UZSKeyboardKeyUserWidget::PressKey()
{
	if(IsValid(WidgetInteractionComponent))
	{
		if(ESpecialKey::SPECIAL == Key.IsSpecialKey)
		{
			if(Key.SpecialKey == FKey("LeftShift"))
			{
				if(IsValid(VirtualKeyboardActor))
				{
					VirtualKeyboardActor->SetIsPressedShift(!VirtualKeyboardActor->bIsPressedShift);
				}
			}
			WidgetInteractionComponent->PressKey(Key.SpecialKey);
		}
		else if(ESpecialKey::NORMAL == Key.IsSpecialKey)
		{
			if(IsValid(VirtualKeyboardActor))
			{
				if(false == VirtualKeyboardActor->bIsPressedShift)
				{
					WidgetInteractionComponent->SendKeyChar(Key.Key);
				}
				else
				{
					WidgetInteractionComponent->SendKeyChar(Key.ShiftPressedKey);
				}
			}
		}
			
	}
}
