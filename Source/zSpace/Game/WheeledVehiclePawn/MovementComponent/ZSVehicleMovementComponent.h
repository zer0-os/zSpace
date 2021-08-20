// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "zSpace/Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"

#include "ZSVehicleMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSVehicleMovementComponent : public UChaosWheeledVehicleMovementComponent
{
	GENERATED_BODY()

	AZSWheeledVehiclePawn * GetVehiclePawn() const;
	
    UFUNCTION(BlueprintCallable)
    int32 GetVehicleSpeed();
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetIdleBrakeInput(const float & NewInput);
};
