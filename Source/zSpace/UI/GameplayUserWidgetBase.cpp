// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/GameplayUserWidgetBase.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerState.h>
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

void UGameplayUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer<APlayerController>();
	if (IsValid(PC))
	{
		APlayerState* PS = PC->GetPlayerState<APlayerState>();
		FString PlayerName = PS->GetPlayerName();
		SetPlayerName(FText::FromString(PlayerName));
	}
}

