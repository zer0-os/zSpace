// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/Accessories/SteeringWheelStaticMeshComponent/SteeringWheelStaticMeshComponent.h"

#include "Engine/CollisionProfile.h"

USteeringWheelStaticMeshComponent::USteeringWheelStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
}

void USteeringWheelStaticMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USteeringWheelStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CalculateRotation(DeltaTime);
}

void USteeringWheelStaticMeshComponent::CalculateRotation(float DeltaTime)
{
	if(ROLE_Authority == GetOwnerRole())
	{
		Current = FMath::FInterpTo(Current, Target, DeltaTime, InterpSpeed);
		//UE_LOG(LogTemp, Log, TEXT("*************** Current = %f ***************"), Current);
		FRotator Rotator(0);
		if(ESteeringWheelRotationAxis::Pitch == RotationAxisWheel)
		{
			Rotator.Pitch = Current;	
		}
		else if(ESteeringWheelRotationAxis::Yaw == RotationAxisWheel)
		{
			Rotator.Yaw = Current;	
		}
		else if(ESteeringWheelRotationAxis::Roll == RotationAxisWheel)
		{
			Rotator.Roll = Current;	
		}
		SetRelativeRotation(Rotator);
	}
}

void USteeringWheelStaticMeshComponent::SetTarget(const float& NewTarget)
{
	Target = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(MinTargetAngle, MaxTargetAngle), NewTarget);
}
