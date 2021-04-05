// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/GameplayUserWidgetBase.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerState.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UGameplayUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	check(HealthProgressBar);
	if (IsValid(HealthProgressBar))
	{
		HealthProgressBar->SetPercent(1.f);
	}
	check(StaminaProgressBar);
	if (IsValid(StaminaProgressBar))
	{
		StaminaProgressBar->SetPercent(1.f);
	}
}

void UGameplayUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer<APlayerController>();
	if (IsValid(PC))
	{
		APlayerState* PS = PC->GetPlayerState<APlayerState>();
		if (IsValid(PS))
		{
			FString PlayerName = PS->GetPlayerName();
			SetPlayerName(FText::FromString(PlayerName));
		}
	}
}

void UGameplayUserWidgetBase::SetHealth(const float CurrentValue, const float MaxValue)
{
	if (!IsValid(HealthTextValue)) return;

	HealthProgressBar->SetPercent(CurrentValue / MaxValue);

	const FString StringCurrentValue = FString::FromInt(UKismetMathLibrary::Round(CurrentValue));
	const FString StringMaxValue = FString::FromInt(UKismetMathLibrary::Round(MaxValue));

	const FString HealthValue = StringCurrentValue + "/" + StringMaxValue;
	HealthTextValue->SetText(FText::FromString(HealthValue));
}

void UGameplayUserWidgetBase::SetStamina(const float CurrentValue, const float MaxValue)
{
	if (!IsValid(StaminaProgressBar)) return;

	StaminaProgressBar->SetPercent(CurrentValue / MaxValue);
}

