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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FExhaustPipeSmokeParticle ExhaustSmokeParticle;

	UPROPERTY()
	class UZSVehicleMovementComponent* OwnerMovementComponent;

	TArray<class UParticleSystemComponent*> SmokeParticleComponents;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	void AdjustSmokeIntensityScale();
public:
	UExhaustPipeComponent();
};
