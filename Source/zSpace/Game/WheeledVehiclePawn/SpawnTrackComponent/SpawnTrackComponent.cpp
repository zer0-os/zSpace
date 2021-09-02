// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/SpawnTrackComponent/SpawnTrackComponent.h"

#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/DataAsset/TrackDataAsset/TrackDataAsset.h"
#include "Game/WheeledVehiclePawn/SurfaceTypesDetectActorComponent/SurfaceTypesDetectActorComponent.h"

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
	if(ROLE_Authority > GetOwnerRole())
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

void USpawnTrackComponent::ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial)
{
	if(IsValid(NewPhysicalMaterial))
	{
		TEnumAsByte<EPhysicalSurface> L_Surface = NewPhysicalMaterial->SurfaceType;
		if (IsValid(TrackDataAsset))
		{	
			UParticleSystem * L_Particle = TrackDataAsset->GetParticle(L_Surface);
			UE_LOG(LogTemp, Warning, TEXT("************* Surface Type = %s **********************"), *UEnum::GetValueAsString<EPhysicalSurface>(L_Surface));
		}
	}
	
}


