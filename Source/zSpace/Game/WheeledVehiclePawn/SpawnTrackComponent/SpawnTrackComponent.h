// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnTrackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API USpawnTrackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnTrackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsSetParticleLocation();

	void SetParticleLocation();

	
	FVector GetWheelLocation(int32 NewIndex);
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UTrackDataAsset * TrackDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<UParticleSystemComponent *> SpawnedTrackes;

	
	class AZSWheeledVehiclePawn * GetVehiclePawn();
	
	class UZSVehicleMovementComponent * GetVehicleMovementComponent();

	void BindSurfaceDetection();

	
public:
	
	UFUNCTION()
	void ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial, int32 I);

	UFUNCTION()
	void SpawnTrack(UParticleSystem * NewParticle, FVector Location, FName SocketName, int32 NewIndex);
	
		
};
