// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "StopLightGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UStopLightGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	static FName VehicleStopLightParamName;
	
public:
	
	UStopLightGameplayAbility();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float StopEmissiveColorStopLightsMaxValue = 500;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRepIsStopLight, meta = (AllowPrivateAccess=true))
	uint8 bIsStopLight:1;

	USkeletalMeshComponent * GetVehicleSkeletalMeshComponent();

public:
	
	UFUNCTION()
	void OnRepIsStopLight();

	UFUNCTION(BlueprintCallablen)
	void SetIsStopLight(bool NewIsStopLight);

	virtual void GetLifetimeReplicatedProps ( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
};
