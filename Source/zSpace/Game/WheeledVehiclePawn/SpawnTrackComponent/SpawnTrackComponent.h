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
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UTrackDataAsset * TrackDataAsset = nullptr;

	class AZSWheeledVehiclePawn * GetVehiclePawn();

	void BindSurfaceDetection();
	
public:
	
	UFUNCTION()
	void ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial);
	
		
};
