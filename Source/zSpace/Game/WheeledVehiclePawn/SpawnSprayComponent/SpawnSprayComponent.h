// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnSprayComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API USpawnSprayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnSprayComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsSetParticleLocation();

	void SetParticleLocation(bool bForceSet = false);

	FVector GetWheelLocation(int32 NewIndex);
	
	private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UTrackDataAsset * TrackDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<UParticleSystemComponent *> SpawnedSpraies;

	
	class AZSWheeledVehiclePawn * GetVehiclePawn();
	
	class UZSVehicleMovementComponent * GetVehicleMovementComponent();

	void BindSurfaceDetection();

	
	public:
	
	UFUNCTION()
	void ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial, int32 I);

	UFUNCTION()
	void SpawnSpray(UParticleSystem * NewParticle, FVector Location, FName SocketName, int32 NewIndex, const FRotator& NewRotation);

	UFUNCTION()
	void SetTemplate(UParticleSystem * NewParticle, int32 NewIndex);
	
		
		
};
