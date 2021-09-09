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
	class UAudioComponent* RumbleSoundComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class USoundWave*> RumbleSounds;

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

	bool GetProbableBit(float Percentage) const;
	class USoundWave* GetRandomRumbleSound()	const;
public:
	UExhaustPipeComponent();
};
