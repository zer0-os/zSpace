// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZSSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	friend class AZSWheeledVehiclePawn;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	uint8 bIsUseControllerInput:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	uint8 bIsUseControllerInputPitch:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	uint8 bIsUseControllerInputYaw:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	uint8 bIsUseControllerInputRoll:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	FRotator MinLimitRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="UseControllerInput")
	FRotator MaxLimitRotation;
	
	static float PlayerCameraManagerViewPitchMin;
	static float PlayerCameraManagerViewPitchMax; 
	static float PlayerCameraManagerViewRollMin;	
	static float PlayerCameraManagerViewRollMax;
	static float PlayerCameraManagerViewYawMin;	
	static float PlayerCameraManagerViewYawMax;
	

public:
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateSpringArmRotation();

	UFUNCTION(BlueprintCallable)
	void SetLimitation(FRotator NewMinLimitRotation, FRotator NewMaxLimitRotation);

	UFUNCTION(BlueprintCallable)
	void UpdateLimitation();

	UFUNCTION(BlueprintCallable)
	void ResetPlayerCameraManagerRotationLimit();

	
	
};

