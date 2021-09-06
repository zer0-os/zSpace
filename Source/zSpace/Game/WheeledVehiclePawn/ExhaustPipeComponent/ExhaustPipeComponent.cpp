// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/ExhaustPipeComponent/ExhaustPipeComponent.h"
#include "Particles/ParticleSystem.h"
#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/StaticMeshSocket.h"

UParticleSystem* FExhaustPipeSmokeParticle::LoadSmokeParticle() const
{
	UParticleSystem* L_ResultSmokeParticle = SmokeParticle.LoadSynchronous();
	return L_ResultSmokeParticle;
}

void UExhaustPipeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Owner->bIsEngineStarted)
		AdjustSmokeIntensityScale();
}

void UExhaustPipeComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AZSWheeledVehiclePawn>(GetOwner());
	OwnerMovementComponent = Owner->GetZSVehicleMovementComponent();

	for (FName IterSocketName : GetAllSocketNames())
	{
		UParticleSystemComponent* PCS = 
										UGameplayStatics::SpawnEmitterAttached(
													ExhaustSmokeParticle.LoadSmokeParticle(),
												    this, 
													IterSocketName, 
													GetSocketLocation(IterSocketName),
												    GetSocketRotation(IterSocketName), 
													EAttachLocation::KeepWorldPosition,
													false);
		SmokeParticleComponents.Add(PCS);
	}
}

void UExhaustPipeComponent::AdjustSmokeIntensityScale()
{
	static float L_MaxRotationSpeed = OwnerMovementComponent->GetEngineMaxRotationSpeed();

	static FVector2D L_InRange(0.f, L_MaxRotationSpeed), L_OutRange(0.f, 1.f);

	float L_SmokeScale = FMath::GetMappedRangeValueClamped(L_InRange, L_OutRange, OwnerMovementComponent->GetEngineRotationSpeed());

	for (UParticleSystemComponent* ParticleCompIter : SmokeParticleComponents)
	{
		ParticleCompIter->SetVectorParameter(FName("Scale"), FVector(L_SmokeScale, L_SmokeScale, L_SmokeScale));
	}
}

UExhaustPipeComponent::UExhaustPipeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
