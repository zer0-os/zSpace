// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacterMovementComponent.h"
#include "ZSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSCharacterMovementComponent : public UOWSCharacterMovementComponent
{
	GENERATED_BODY()

public:
	
	UZSCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer);

	virtual void OnMovementModeChanged ( EMovementMode NewPreviousMovementMode, uint8 NewPreviousCustomMode ) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementModeChanged, EMovementMode, PreviousMovementMode, EMovementMode, CurrentMovementMode,  uint8, PreviousCustomMode);

	UPROPERTY(BlueprintAssignable)
	FMovementModeChanged OnMovementModeChangedDelegate;
	
};
