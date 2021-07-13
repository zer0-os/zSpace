// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "StopRearLightAbilityTask.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UStopRearLightAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UStopRearLightAbilityTask();
	
	virtual void TickTask(float DeltaTime) override;
	
};


