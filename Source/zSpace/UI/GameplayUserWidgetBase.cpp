// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/GameplayUserWidgetBase.h"
#include <Components/ProgressBar.h>

void UGameplayUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(HealthProgressBar))
	{
		HealthProgressBar->SetPercent(1.f);
	}
	if (IsValid(StaminaProgressBar))
	{
		StaminaProgressBar->SetPercent(1.f);
	}
}

EWidgetType UGameplayUserWidgetBase::GetWidgetType_Implementation()
{
	return EWidgetType::Gameplay;
}
