// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/ZSCustomButton.h"

void UZSCustomButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	OnClicked.AddUniqueDynamic(this, &UZSCustomButton::OnClickedEvent);
}

void UZSCustomButton::OnClickedEvent()
{
	OnClickedReturnThis.Broadcast(this);
}

void UZSCustomButton::SetResolution(const EResolution NewValue)
{
	Resolution = NewValue;
}
