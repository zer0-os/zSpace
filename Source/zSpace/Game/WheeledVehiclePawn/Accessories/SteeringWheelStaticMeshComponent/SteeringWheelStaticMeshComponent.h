// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SteeringWheelStaticMeshComponent.generated.h"


UENUM(BlueprintType, Blueprintable)
enum class ESteeringWheelRotationAxis : uint8
{
	Pitch
	,Yaw
	,Roll
};

/**
 * 
 */
UCLASS()
class ZSPACE_API USteeringWheelStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	
	USteeringWheelStaticMeshComponent();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	ESteeringWheelRotationAxis RotationAxisWheel = ESteeringWheelRotationAxis::Roll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float InterpSpeed = 10;

	UPROPERTY(Transient)
	float Current = 0;

	UPROPERTY(Transient)
	float Target = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float MaxTargetAngle = 540;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float MinTargetAngle = -540;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CalculateRotation(float DeltaTime);
	
public:
	
	void SetTarget(const float & NewTarget);

	
	
};
