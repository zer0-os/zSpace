// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/SpawnTrackComponent/SpawnTrackComponent.h"
#include "SComponentClassCombo.h"
#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/DataAsset/TrackDataAsset/TrackDataAsset.h"
#include "Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"
#include "Game/WheeledVehiclePawn/SurfaceTypesDetectActorComponent/SurfaceTypesDetectActorComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnTrackComponent::USpawnTrackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnTrackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(ROLE_Authority > GetOwnerRole() || UKismetSystemLibrary::IsStandalone(this))
	{
		BindSurfaceDetection();
	}
}


// Called every frame
void USpawnTrackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AZSWheeledVehiclePawn * USpawnTrackComponent::GetVehiclePawn()
{
	return 	Cast<AZSWheeledVehiclePawn>(GetOwner());
}

void USpawnTrackComponent::BindSurfaceDetection()
{
	AZSWheeledVehiclePawn * Pawn = GetVehiclePawn();
	if(IsValid(Pawn))
	{
		USurfaceTypesDetectActorComponent * L_SurfaceTypesDetectActorComponent = Cast<USurfaceTypesDetectActorComponent>(Pawn->GetComponentByClass(USurfaceTypesDetectActorComponent::StaticClass()));
		if(IsValid(L_SurfaceTypesDetectActorComponent))
		{
			L_SurfaceTypesDetectActorComponent->OnClientSurfaceTypeChange.AddUniqueDynamic(this, &USpawnTrackComponent::ClientSurfaceTypeChange);
		}
	}
}

void USpawnTrackComponent::ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial, int32 I)
{
	if(IsValid(NewPhysicalMaterial))
	{
		TEnumAsByte<EPhysicalSurface> L_Surface = NewPhysicalMaterial->SurfaceType;
		if (IsValid(TrackDataAsset))
		{	
			UParticleSystem * L_Particle = TrackDataAsset->GetParticle(L_Surface);
			TrackLocation(L_Particle);
		}
	}
}

UZSVehicleMovementComponent* USpawnTrackComponent::GetVehicleMovementComponent()
{
	UZSVehicleMovementComponent * R_Movement = nullptr;
	AZSWheeledVehiclePawn * Vehicle = GetVehiclePawn();
	if(IsValid(Vehicle))
	{
		R_Movement = Cast<UZSVehicleMovementComponent>(Vehicle->GetVehicleMovementComponent());	
	}
	return R_Movement;
}

 void USpawnTrackComponent::SpawnTrack(UParticleSystem * Particle, FVector Location, FName SocketName)
{
	UParticleSystemComponent * L_SpawnParticle = nullptr;
	AZSWheeledVehiclePawn * Vehicle = GetVehiclePawn();
	USkeletalMeshComponent * L_Mesh = Vehicle->GetMesh();
	L_SpawnParticle = UGameplayStatics::SpawnEmitterAttached(Particle, L_Mesh, SocketName, Location) ;
}

void USpawnTrackComponent::TrackLocation(UParticleSystem * Particle)
{
	UZSVehicleMovementComponent * Movement = GetVehicleMovementComponent();
    	if(IsValid(Movement))
    	{
    		const int32 L_WheelCount = Movement->GetNumWheels();	
    		for(int32 I = 0; I < L_WheelCount; I++)
    		{
    			FVector Location = Movement->Wheels[I]->Location;
    			FName L_Name = Movement->WheelSetups[I].BoneName;
				SpawnTrack(Particle, Location, L_Name);
    		}
    	}
}


