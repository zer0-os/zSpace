// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/ExhaustPipeComponent/ExhaustPipeComponent.h"
#include "Particles/ParticleSystem.h"
#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "ExhaustPipeComponent.h"

UExhaustPipeComponent::UExhaustPipeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ThrottleSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ThrottleSoundComponent"));
	ThrottleSoundComponent->SetupAttachment(this);
	ThrottleSoundComponent->bAutoActivate = false;
}
UParticleSystem* FExhaustPipeSmokeParticle::LoadSmokeParticle() const
{
	UParticleSystem* L_ResultSmokeParticle = SmokeParticle.LoadSynchronous();
	return L_ResultSmokeParticle;
}

void UExhaustPipeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(Owner))
	{
		return;
	}

	AdjustSmokeIntensityScale(Owner->bIsEngineStarted);
	AttemptPlayThrottleSound();
}

void UExhaustPipeComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AZSWheeledVehiclePawn>(GetOwner());
	if(IsValid(Owner))
	{
		OwnerMovementComponent = Owner->GetZSVehicleMovementComponent();
		for (FName IterSocketName : GetAllSocketNames())
		{
			UParticleSystemComponent * PCS = UGameplayStatics::SpawnEmitterAttached(
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
}

void UExhaustPipeComponent::AdjustSmokeIntensityScale(bool EngineStarted)
{
	static float L_MaxRotationSpeed = OwnerMovementComponent->GetEngineMaxRotationSpeed();

	static FVector2D L_InRange(0.f, L_MaxRotationSpeed), L_OutRange(0.f, 1.f);

	float L_SmokeScale = FMath::GetMappedRangeValueClamped(L_InRange, L_OutRange, OwnerMovementComponent->GetEngineRotationSpeed());

	for (UParticleSystemComponent* ParticleCompIter : SmokeParticleComponents)
	{
		if(IsValid(ParticleCompIter))
		{
			ParticleCompIter->SetActive(EngineStarted);
			ParticleCompIter->SetVectorParameter(FName("Scale"), FVector(L_SmokeScale, L_SmokeScale, L_SmokeScale));
			ParticleCompIter->SetVectorParameter(FName("ColorScale"), FVector(L_SmokeScale, L_SmokeScale, L_SmokeScale));
		}
	}
}

void UExhaustPipeComponent::AttemptPlayThrottleSound()
{
	if (!IsValid(ThrottleSoundComponent))
	{
		return;
	}

	static const float L_Threshold = 70.f; // Max Threshold Percentage
	static const float L_RPMLimit  = OwnerMovementComponent->GetEngineMaxRotationSpeed();

	float L_CurrentPercentage = OwnerMovementComponent->GetEngineRotationSpeed() * 100.f / L_RPMLimit;

	if (L_CurrentPercentage >= L_Threshold)
	{
		if (!ThrottleSoundComponent->IsPlaying() && GetProbableBit(50.f))  // 50% chance of playing
		{
			USoundWave* L_CurrentThrottleSound = GetRandomThrottleSound();
			if (IsValid(L_CurrentThrottleSound))
			{
				ThrottleSoundComponent->SetSound(L_CurrentThrottleSound);
				ThrottleSoundComponent->Play();
			}
		}
	}
}

USoundWave* UExhaustPipeComponent::GetRandomThrottleSound() const
{
	USoundWave* L_RandomThrottleSound = ThrottleSounds[FMath::RandRange(0, ThrottleSounds.Num() - 1)];

	return L_RandomThrottleSound;
}

bool UExhaustPipeComponent::GetProbableBit(float Percentage) const
{
	float L_RandomFloat = FMath::RandRange(0.f, 100.f);
	bool  L_Result = L_RandomFloat <= Percentage;

	return L_Result;
}

