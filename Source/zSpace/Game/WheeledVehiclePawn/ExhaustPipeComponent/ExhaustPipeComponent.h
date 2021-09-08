// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ExhaustPipeComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FExhaustPipeSmokeParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UParticleSystem> SmokeParticle;

	UParticleSystem* LoadSmokeParticle() const;
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ZSPACE_API UExhaustPipeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FExhaustPipeSmokeParticle ExhaustSmokeParticle;

	UPROPERTY()
	class UAudioComponent* ThrottleSoundComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<class USoundWave*> ThrottleSounds;

	UPROPERTY()
	class UChaosWheeledVehicleMovementComponent * OwnerMovementComponent = nullptr;

	UPROPERTY()
	class AZSWheeledVehiclePawn* Owner = nullptr;

	TArray<class UParticleSystemComponent*> SmokeParticleComponents;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	void AdjustSmokeIntensityScale(bool EngineStarted);
	void AttemptPlayThrottleSound();

	class USoundWave* GetRandomThrottleSound() const;
public:
	UExhaustPipeComponent();
};
