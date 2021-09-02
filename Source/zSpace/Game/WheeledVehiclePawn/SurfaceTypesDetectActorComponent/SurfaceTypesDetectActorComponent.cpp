// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/SurfaceTypesDetectActorComponent/SurfaceTypesDetectActorComponent.h"
#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"

// Sets default values for this component's properties
USurfaceTypesDetectActorComponent::USurfaceTypesDetectActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USurfaceTypesDetectActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USurfaceTypesDetectActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	CheckUnderWheels();
}

AZSWheeledVehiclePawn* USurfaceTypesDetectActorComponent::GetVehicle()
{
	return Cast<AZSWheeledVehiclePawn>(GetOwner());
}

UZSVehicleMovementComponent* USurfaceTypesDetectActorComponent::GetVehicleMovementComponent()
{
	UZSVehicleMovementComponent * R_Movement = nullptr;
	AZSWheeledVehiclePawn * Vehicle = GetVehicle();
	if(IsValid(Vehicle))
	{
		 R_Movement = Cast<UZSVehicleMovementComponent>(Vehicle->GetVehicleMovementComponent());	
	}
	return R_Movement;
}

void USurfaceTypesDetectActorComponent::CheckUnderWheels()
{
	UZSVehicleMovementComponent * Movement = GetVehicleMovementComponent();
	if(IsValid(Movement))
	{
		const int32 L_WheelCount = Movement->GetNumWheels();	
		for(int32 I = 0; I < L_WheelCount; I++)
		{
			UPhysicalMaterial * L_WheelPhysicsMaterial =Movement->Wheels[I]->GetContactSurfaceMaterial();
			if(!ContactSurfaceMaterial.IsValidIndex(I) || ContactSurfaceMaterial[I] != L_WheelPhysicsMaterial)
			{
				if(!ContactSurfaceMaterial.IsValidIndex(I))
				{
					ContactSurfaceMaterial.Insert(L_WheelPhysicsMaterial, I);
				}
				else
				{
					ContactSurfaceMaterial[I] = L_WheelPhysicsMaterial;
				}
				if(IsValid(L_WheelPhysicsMaterial))
				{
					UE_LOG(LogTemp, Warning, TEXT("************* Surface Type = %s"), *UEnum::GetValueAsString<EPhysicalSurface>(L_WheelPhysicsMaterial->SurfaceType));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("************* Surface Type is default"));
				}
				if(ROLE_Authority == GetOwnerRole())
				{
					OnServerSurfaceTypeChange.Broadcast(L_WheelPhysicsMaterial);
				}
				else if(ROLE_Authority > GetOwnerRole())
				{
					OnClientSurfaceTypeChange.Broadcast(L_WheelPhysicsMaterial);
				}
			}
		}
			
	}
}



