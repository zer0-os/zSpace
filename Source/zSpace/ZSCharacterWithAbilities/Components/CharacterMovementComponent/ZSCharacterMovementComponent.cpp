// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"
#include "Engine/EngineTypes.h"

UZSCharacterMovementComponent::UZSCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UZSCharacterMovementComponent::OnMovementModeChanged(EMovementMode NewPreviousMovementMode, uint8 NewPreviousCustomMode)
{
	//FString L_CharacterMode = UEnum::GetValueAsString<EMovementMode>(PreviousMovementMode);
	//UE_LOG(LogTemp, Warning, TEXT("************* PreviousMovementMode = %s, PreviousCustomMode = %d"), *L_CharacterMode, PreviousCustomMode);
	OnMovementModeChangedDelegate.Broadcast(NewPreviousMovementMode, NewPreviousCustomMode);
	Super::OnMovementModeChanged(NewPreviousMovementMode, NewPreviousCustomMode);
}
