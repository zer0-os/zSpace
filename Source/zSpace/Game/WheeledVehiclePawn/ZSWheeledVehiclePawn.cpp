// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MovementComponent/ZSVehicleMovementComponent.h"

AZSWheeledVehiclePawn::AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UZSVehicleMovementComponent>(AWheeledVehiclePawn::VehicleMovementComponentName))
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	checkf(nullptr != SpringArmComponent, TEXT("The SpringArmComponent is nullptr."));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	checkf(nullptr != CameraComponent, TEXT("The CameraComponent is nullptr."));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	
}

UZSVehicleMovementComponent * AZSWheeledVehiclePawn::GetZSVehicleMovementComponent() const
{
	return  Cast<UZSVehicleMovementComponent>(GetVehicleMovementComponent());
}

void AZSWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis(TEXT("VehicleMoveForward"), this, &AZSWheeledVehiclePawn::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("VehicleMoveRight"), this, &AZSWheeledVehiclePawn::MoveRight);
		PlayerInputComponent->BindAction(TEXT("VehicleHandbrake"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::HandbrakePressed);
		PlayerInputComponent->BindAction(TEXT("VehicleHandbrake"), EInputEvent::IE_Released, this, &AZSWheeledVehiclePawn::HandbrakeReleased);
		PlayerInputComponent->BindAxis(TEXT("VehicleLookUp"),this, &AZSWheeledVehiclePawn::LookUp);
		PlayerInputComponent->BindAxis(TEXT("LookRight"),this, &AZSWheeledVehiclePawn::LookRight);
		
	}
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZSWheeledVehiclePawn::MoveForward(float NewValue)
{
	
}

void AZSWheeledVehiclePawn::MoveRight(float NewValue)
{
	
}

void AZSWheeledVehiclePawn::HandbrakePressed()
{
	
}

void AZSWheeledVehiclePawn::HandbrakeReleased()
{
	
}

void AZSWheeledVehiclePawn::LookUp(float NewValue)
{
	
}

void AZSWheeledVehiclePawn::LookRight(float NewValue)
{
	
}

