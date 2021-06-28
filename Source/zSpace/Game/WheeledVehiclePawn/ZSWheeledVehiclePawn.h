// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ZSWheeledVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USpringArmComponent *  SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UCameraComponent * CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UAudioComponent * EngineSoundComponent = nullptr; 

	UFUNCTION(BlueprintCallable)
	class UZSVehicleMovementComponent * GetZSVehicleMovementComponent() const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UBoxComponent * VehicleZoneBoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class AZSCharacterWithAbilities * 	ZSCharacterWithAbilities = nullptr;
	
	AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetZsCharacterWithAbilities(class AZSCharacterWithAbilities * 	NewZSCharacterWithAbilities);

	
	UFUNCTION(BlueprintCallable)
	class AZSCharacterWithAbilities * GetCharacterWithAbilities();

	UFUNCTION(BlueprintCallable)
	void HideVehicleControlWidget();
	
	UFUNCTION(BlueprintCallable)
	void ShowVehicleControlWidget();
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UVehicleWidgetsDataAsset * VehicleWidgetsDataAsset = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * VehicleControlWidget = nullptr;

		
protected:
	virtual void BeginPlay() override;
	
public:
	
	virtual void Tick(float DeltaSeconds) override;


	
protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:
	
	UFUNCTION()
	void ComponentBeginOverlapVehicleZoneBoxComponent(UPrimitiveComponent * OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void  ComponentEndOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	UFUNCTION()
	void MoveForward(float NewValue);

	UFUNCTION()
	void MoveRight(float NewValue);

	UFUNCTION()
	void HandbrakePressed();
	
	UFUNCTION()
	void HandbrakeReleased();

	UFUNCTION()
	void LookUp(float NewValue);

	UFUNCTION()
	void LookRight(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void LeaveVehicle();
};

