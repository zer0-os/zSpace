// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleWheels/ZSChaosVehicleWheelFront.h"
#include "ChaosVehicleWheels/ZSChaosVehicleWheelRear.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MovementComponent/ZSVehicleMovementComponent.h"

AZSWheeledVehiclePawn::AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UZSVehicleMovementComponent>(AWheeledVehiclePawn::VehicleMovementComponentName))
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	checkf(nullptr != SpringArmComponent, TEXT("The SpringArmComponent is nullptr."));
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArmComponent->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 125.0f;
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->bEnableCameraRotationLag = false;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	checkf(nullptr != CameraComponent, TEXT("The CameraComponent is nullptr."));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	CameraComponent->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = 90.f;

	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	//EngineSoundComponent->SetSound(SoundCue.Object); // TODO need add Engine Sound dynamic
	EngineSoundComponent->SetupAttachment(GetMesh());

	UChaosWheeledVehicleMovementComponent * VehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	// Wheels/Tyres
	// Setup the wheels
	VehicleMovement->WheelSetups.SetNum(4);
	{
		VehicleMovement->WheelSetups[0].WheelClass = UZSChaosVehicleWheelFront::StaticClass();
		VehicleMovement->WheelSetups[0].BoneName = FName("PhysWheel_FL");
		VehicleMovement->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

		VehicleMovement->WheelSetups[1].WheelClass = UZSChaosVehicleWheelFront::StaticClass();
		VehicleMovement->WheelSetups[1].BoneName = FName("PhysWheel_FR");
		VehicleMovement->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

		VehicleMovement->WheelSetups[2].WheelClass = UZSChaosVehicleWheelRear::StaticClass();
		VehicleMovement->WheelSetups[2].BoneName = FName("PhysWheel_BL");
		VehicleMovement->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

		VehicleMovement->WheelSetups[3].WheelClass = UZSChaosVehicleWheelRear::StaticClass();
		VehicleMovement->WheelSetups[3].BoneName = FName("PhysWheel_BR");
		VehicleMovement->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);
	}

	// Engine 
	// Torque setup
	VehicleMovement->EngineSetup.MaxRPM = 5700.0f;
	VehicleMovement->EngineSetup.MaxTorque = 500.0f;
	VehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	VehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	VehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	VehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	// This works because the AxleType has been setup on the wheels
	VehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;

	// Adjust the steering 
	VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	// Drive the front wheels a little more than the rear
	VehicleMovement->DifferentialSetup.FrontRearSplit = 0.65;

	// Automatic gearbox
	VehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
	VehicleMovement->TransmissionSetup.bUseAutoReverse = true;
	VehicleMovement->TransmissionSetup.GearChangeTime = 0.15f;

	// Physics settings
	// Adjust the center of mass - the buggy is quite low
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(VehicleMovement->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, -15.0f);
	}

	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	VehicleMovement->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);

}

void AZSWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	// Start an engine sound playing
	EngineSoundComponent->Play(); // TODO Need to set Engine Sound.
}

void AZSWheeledVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UZSVehicleMovementComponent * AZSWheeledVehiclePawn::GetZSVehicleMovementComponent() const
{
	return  Cast<UZSVehicleMovementComponent>(GetVehicleMovementComponent());
}

void AZSWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	checkf(nullptr != PlayerInputComponent, TEXT("The PlayerInputComponent is nullptr."));
	if(IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis(TEXT("VehicleMoveForward"), this, &AZSWheeledVehiclePawn::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("VehicleMoveRight"), this, &AZSWheeledVehiclePawn::MoveRight);
		PlayerInputComponent->BindAction(TEXT("VehicleHandbrake"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::HandbrakePressed);
		PlayerInputComponent->BindAction(TEXT("VehicleHandbrake"), EInputEvent::IE_Released, this, &AZSWheeledVehiclePawn::HandbrakeReleased);
		PlayerInputComponent->BindAxis(TEXT("VehicleLookUp"),this, &AZSWheeledVehiclePawn::LookUp);
		PlayerInputComponent->BindAxis(TEXT("LookRight"),this, &AZSWheeledVehiclePawn::LookRight);
		
	}
}

void AZSWheeledVehiclePawn::MoveForward(float NewValue)
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		if(NewValue > 0)
		{
			L_VehicleMovementComponent->SetThrottleInput(NewValue);
			L_VehicleMovementComponent->SetBrakeInput(0.f);
		}
		else
		{
			L_VehicleMovementComponent->SetThrottleInput(0.f);
			L_VehicleMovementComponent->SetBrakeInput(-NewValue);
		}
	}
}

void AZSWheeledVehiclePawn::MoveRight(float NewValue)
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->SetSteeringInput(NewValue);
	}
}

void AZSWheeledVehiclePawn::HandbrakePressed()
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->SetHandbrakeInput(true);
	}
}

void AZSWheeledVehiclePawn::HandbrakeReleased()
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->SetHandbrakeInput(false);
	}
}

void AZSWheeledVehiclePawn::LookUp(float NewValue)
{
	AddControllerRollInput(NewValue);
}

void AZSWheeledVehiclePawn::LookRight(float NewValue)
{
	AddControllerYawInput(NewValue);
}

