// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurfaceTypesDetectActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API USurfaceTypesDetectActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USurfaceTypesDetectActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	class AZSWheeledVehiclePawn * GetVehicle();

	class UZSVehicleMovementComponent * GetVehicleMovementComponent();


	UPROPERTY()
	TArray<UPhysicalMaterial *>  ContactSurfaceMaterial;

	void CheckUnderWheels();
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSurfaceTypeChange, UPhysicalMaterial *, NewPhysicalMaterial);

	UPROPERTY(BlueprintAssignable)
	FServerSurfaceTypeChange OnServerSurfaceTypeChange;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientSurfaceTypeChange, UPhysicalMaterial *, NewPhysicalMaterial);
	
	UPROPERTY(BlueprintAssignable)
	FClientSurfaceTypeChange OnClientSurfaceTypeChange;

	
};
