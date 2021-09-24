// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleWheels/ZSChaosVehicleWheelFront.h"
#include "ChaosVehicleWheels/ZSChaosVehicleWheelRear.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbility/AttributeSet/ZSVehicleAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MovementComponent/ZSVehicleMovementComponent.h"
#include "VehicleWIdgetsDataAsset/VehicleWIdgetsDataAsset.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/Game/ZSCameraComponent/ZSCameraComponent.h"
#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpawnTrackComponent/SpawnTrackComponent.h"
#include "SpringArmComponent/ZSSpringArmComponent.h"
#include "SurfaceTypesDetectActorComponent/SurfaceTypesDetectActorComponent.h"
#include "zSpace/Game/ZSGamePlayerController/ZSGamePlayerController.h"
#include "zSpace/ZSCharacterWithAbilities/Components/ManageCharacterMeshComponent/ManageCharacterMeshAC.h"
#include "AIController.h"
#include "GameFramework/PlayerState.h"
#include "Particles/ParticleSystemComponent.h"

FName AZSWheeledVehiclePawn::VehicleStopLightParamName = "EmissiveColorStopLights";

FName AZSWheeledVehiclePawn::VehicleRearLightParamName = "EmissiveColorRearLights";

FName AZSWheeledVehiclePawn::VehicleFrontAndRearLightsParamName = "EmissiveColorFrontAndRearLights";

FName AZSWheeledVehiclePawn::VehicleInSideLight = "LightOn";


AZSWheeledVehiclePawn::AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UZSVehicleMovementComponent>(AWheeledVehiclePawn::VehicleMovementComponentName))
{

	DriverBoneNameForHHide.Add("head");
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	checkf(nullptr != AbilitySystemComponent, TEXT("The AbilitySystemComponent is nullptr."));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetCullDistanceSquared = 999000000;
	NetUpdateFrequency = 200;
	MinNetUpdateFrequency = 50;
	NetPriority = 3;
	FRepMovement L_RepMovement = GetReplicatedMovement();
	L_RepMovement.VelocityQuantizationLevel = EVectorQuantization::RoundTwoDecimals;
	L_RepMovement.RotationQuantizationLevel = ERotatorQuantization::ShortComponents;
	SetReplicatedMovement(L_RepMovement);

	SkeletalMeshComponentDriver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentDriver"));
	checkf(nullptr != SkeletalMeshComponentDriver, TEXT("The SkeletalMeshComponentDriver is nullptr."));
	SkeletalMeshComponentDriver->SetupAttachment(RootComponent);
	SkeletalMeshComponentDriver->SetMobility(EComponentMobility::Movable);

	SteeringWheelStaticMeshComponent = CreateDefaultSubobject<USteeringWheelStaticMeshComponent>(TEXT("SteeringWheelStaticMeshComponent"));
	checkf(nullptr != SteeringWheelStaticMeshComponent, TEXT("The SteeringWheelStaticMeshComponent is nullptr."));
	SteeringWheelStaticMeshComponent->SetupAttachment(RootComponent);
	SteeringWheelStaticMeshComponent->RotationAxisWheel = ESteeringWheelRotationAxis::Roll;
	SteeringWheelStaticMeshComponent->InterpSpeed = 1;
	SteeringWheelStaticMeshComponent->MaxTargetAngle = 80;
	SteeringWheelStaticMeshComponent->MinTargetAngle = -80;
	SteeringWheelStaticMeshComponent->SetIsReplicated(true);
	SteeringWheelStaticMeshComponent->SetMobility(EComponentMobility::Movable);
	
	
	SpringArmComponentDefault = CreateDefaultSubobject<UZSSpringArmComponent>(TEXT("SpringArmComponentDefault"));
	checkf(nullptr != SpringArmComponentDefault, TEXT("The SpringArmComponent is nullptr."));
	SpringArmComponentDefault->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArmComponentDefault->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArmComponentDefault->SetupAttachment(RootComponent);
	SpringArmComponentDefault->TargetArmLength = 125.0f;
	SpringArmComponentDefault->bEnableCameraLag = false;
	SpringArmComponentDefault->bEnableCameraRotationLag = false;
	SpringArmComponentDefault->bInheritPitch = true;
	SpringArmComponentDefault->bInheritYaw = true;
	SpringArmComponentDefault->bInheritRoll = true;

	CameraComponentDefault = CreateDefaultSubobject<UZSCameraComponent>(TEXT("CameraComponentDefault"));
	checkf(nullptr != CameraComponentDefault, TEXT("The CameraComponentDefault is nullptr."));
	CameraComponentDefault->SetupAttachment(SpringArmComponentDefault, USpringArmComponent::SocketName);
	CameraComponentDefault->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	CameraComponentDefault->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	CameraComponentDefault->bUsePawnControlRotation = false;
	CameraComponentDefault->PostProcessSettings.MotionBlurAmount = 0;
	CameraComponentDefault->FieldOfView = 90.f;
	CameraComponentDefault->SetCameraPositionType(ECameraPositionType::DefaultCamera);
	
	SpringArmComponentInSide = CreateDefaultSubobject<UZSSpringArmComponent>(TEXT("SpringArmComponentInSide"));
	checkf(nullptr != SpringArmComponentInSide, TEXT("The SpringArmComponentInSide is nullptr."));
	SpringArmComponentInSide->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArmComponentInSide->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArmComponentInSide->SetupAttachment(RootComponent);
	SpringArmComponentInSide->TargetArmLength = 0;
	SpringArmComponentInSide->bEnableCameraLag = false;
	SpringArmComponentInSide->bEnableCameraRotationLag = false;
	SpringArmComponentInSide->bInheritPitch = true;
	SpringArmComponentInSide->bInheritYaw = true;
	SpringArmComponentInSide->bInheritRoll = true;

	CameraComponentInSide = CreateDefaultSubobject<UZSCameraComponent>(TEXT("CameraComponentInSide"));
	checkf(nullptr != CameraComponentInSide, TEXT("The CameraComponentInSide is nullptr."));
	CameraComponentInSide->SetupAttachment(SpringArmComponentInSide, USpringArmComponent::SocketName);
	CameraComponentInSide->FieldOfView = 90.f;
	CameraComponentInSide->SetCameraPositionType(ECameraPositionType::CameraInSide);
	CameraComponentInSide->PostProcessSettings.MotionBlurAmount = 0;
	

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

	VehicleZoneBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VehicleZoneBoxComponent"));
	checkf(nullptr != VehicleZoneBoxComponent, TEXT("The VehicleZoneBoxComponent is nullptr."));
	VehicleZoneBoxComponent->SetupAttachment(RootComponent);
	VehicleZoneBoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	VehicleZoneBoxComponent->SetBoxExtent(FVector(86, 150, 36));
	VehicleZoneBoxComponent->SetRelativeLocation(FVector(0,0,40));
	VehicleZoneBoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AZSWheeledVehiclePawn::ComponentBeginOverlapVehicleZoneBoxComponent);
	VehicleZoneBoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AZSWheeledVehiclePawn::ComponentEndOverlapVehicleZoneBoxComponent);

	SpotLightComponentFrontLeftLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponentFrontLeftLight"));
	checkf(nullptr != SpotLightComponentFrontLeftLight, TEXT("The SpotLightComponentFrontLeftLight is nullptr."));
	SpotLightComponentFrontLeftLight->SetupAttachment(RootComponent);

	SpotLightComponentFrontRightLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponentFrontRightLight"));
	checkf(nullptr != SpotLightComponentFrontRightLight, TEXT("The SpotLightComponentFrontRightLight is nullptr."));
	SpotLightComponentFrontRightLight->SetupAttachment(RootComponent);
	
	Speedometer3D = CreateDefaultSubobject<UWidgetComponent>(TEXT("Speedometer3D"));
	Speedometer3D->SetupAttachment(RootComponent);
	Speedometer3D->SetVisibility(true);

	//
	SurfaceTypesDetectActorComponent = CreateDefaultSubobject<USurfaceTypesDetectActorComponent>(TEXT("SurfaceTypesDetectActorComponent"));
	checkf(nullptr != SurfaceTypesDetectActorComponent, TEXT("The SurfaceTypesDetectActorComponent is nullptr. "));
	SurfaceTypesDetectActorComponent->SetIsReplicated(true);
	AddOwnedComponent(SurfaceTypesDetectActorComponent);
	
	SpawnTrackComponent = CreateDefaultSubobject<USpawnTrackComponent>(TEXT("SpawnTrackComponent"));
	checkf(nullptr != SpawnTrackComponent, TEXT("The SpawnTrackComponent is nullptr. "));
	AddOwnedComponent(SpawnTrackComponent);
	AutoPossessAI =	EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TimerDelegateTurnOff = FTimerDelegate::CreateUObject(this, &AZSWheeledVehiclePawn::CanDisableVehicleMovement);

	/*
	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
	checkf(nullptr != PathFollowingComponent, TEXT("The PathFollowingComponent is nullptr."));
	AddOwnedComponent(PathFollowingComponent);
*/
}

void AZSWheeledVehiclePawn::Server_SetForwardInputValue_Implementation(const float& NewForwardInput)
{
	ForwardInput = NewForwardInput;	
}

bool AZSWheeledVehiclePawn::Server_SetForwardInputValue_Validate(const float& NewForwardInput)
{
	return !(NewForwardInput > 1 || NewForwardInput < -1);
}

void AZSWheeledVehiclePawn::SetIsHiddenDriver(bool NewIsShowDriver)
{
	bIsHiddenDriver = NewIsShowDriver;	
}

void AZSWheeledVehiclePawn::SendForwardInputToServer(const float& NewForwardInput)
{
	if(NewForwardInput != ForwardInput)
	{
		ForwardInput = NewForwardInput;
		Server_SetForwardInputValue(NewForwardInput);
	}
}

void AZSWheeledVehiclePawn::SetZsCharacterWithAbilities(AZSCharacterWithAbilities* NewZSCharacterWithAbilities)
{
	ZSCharacterWithAbilities = NewZSCharacterWithAbilities;
}

AZSCharacterWithAbilities* AZSWheeledVehiclePawn::GetCharacterWithAbilities()
{
	return ZSCharacterWithAbilities;
}

void AZSWheeledVehiclePawn::ShowVehicleControlWidget()
{
	if(ROLE_Authority != GetLocalRole())
	{
		if(IsValid(VehicleWidgetsDataAsset))
		{
			if(nullptr == VehicleControlWidget)
			{
				const TSubclassOf<UUserWidget> L_UserWidgetControllerHelper =  VehicleWidgetsDataAsset->GetUserWidgetControlHelper();
				VehicleControlWidget = CreateWidget<UUserWidget>(GetWorld(), L_UserWidgetControllerHelper);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The VehicleWidgetDataAsset is null"));
		}
		if(IsValid(VehicleControlWidget))
		{
			if(false == VehicleControlWidget->IsInViewport())
			{
				VehicleControlWidget->AddToViewport(15);
			}
		}
	}
}

void AZSWheeledVehiclePawn::HideVehicleControlWidget()
{
	if(ROLE_Authority != GetLocalRole())
	{
		if(IsValid(VehicleControlWidget))
		{
			VehicleControlWidget->RemoveFromViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The VehicleControl Widget is null"));
		}
	}
}

void AZSWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	DisableClientCollisions();
	EObjectFlags L_Flags = GetFlags() | RF_Transient;
	SetFlags(L_Flags);
	SetAutonomousProxy(true);
	if(ROLE_Authority == GetLocalRole())
	{
	    SetIdleBrakeInput(10);
		SetIsHiddenDriver(true);
	}
	// Start an engine sound playing
	EngineSoundComponent->Play(); 
	InitAttributes();
	//MultiCastEnableTick(false);
	OnFrontLights(false);
	SetEngineStart(false);
	OnInSideLight(false);
	TurnOffVehicle();
}

void AZSWheeledVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(false == IsTickWorks())
	{
		if(ROLE_Authority == GetLocalRole())
		{
			CheckVehicleStop();
		}
		EngineAudioProcessing();	
	}
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
		PlayerInputComponent->BindAction(TEXT("LeaveVehicle"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::LeaveVehicle);
		PlayerInputComponent->BindAction(TEXT("EnableFrontLight"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::EnableFrontLight);
		
		PlayerInputComponent->BindAction(TEXT("VehicleNextCamera"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::VehicleNextCamera);
		PlayerInputComponent->BindAction(TEXT("VehicleBackCamera"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::VehicleBackCamera);
		PlayerInputComponent->BindAction(TEXT("RectifyVehicle"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::Server_OnRectifyVehicle);
		
		
	}
}

void AZSWheeledVehiclePawn::Server_OnRectifyVehicle_Implementation()
{
	FRotator Rotation = GetActorRotation();
	const float Angle = UKismetMathLibrary::Abs(Rotation.Roll);
	if(Angle > 65)
	{
		Rotation.Roll = 0;
		SetActorRotation(Rotation, ETeleportType::TeleportPhysics);
	}
}

bool AZSWheeledVehiclePawn::Server_OnRectifyVehicle_Validate()
{
	return true;
}


bool AZSWheeledVehiclePawn::IsEnterVehicle(AActor* NewOtherActor)
{
	if(ROLE_Authority == GetLocalRole())
	{
		AZSCharacterWithAbilities * Character = Cast<AZSCharacterWithAbilities>(NewOtherActor);
		if(IsValid(Character))
		{
			AZSGamePlayerController * PC = Cast<AZSGamePlayerController>(GetController());
			if(nullptr == PC )
			{
				return true;	
			}
		}
	}
	return false;
}


void AZSWheeledVehiclePawn::ComponentBeginOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ROLE_Authority == GetLocalRole())
	{
		UCapsuleComponent * CapsuleComponent = Cast<UCapsuleComponent>(OtherComp);
		AZSCharacterWithAbilities * Character = Cast<AZSCharacterWithAbilities>(OtherActor);
		if(IsValid(Character) && Character->GetCapsuleComponent() == CapsuleComponent && IsEnterVehicle(OtherActor))
		{
			Character->ShowEnterVehicleWidget(this);
		}
	}
}

void AZSWheeledVehiclePawn::ComponentEndOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZSCharacterWithAbilities * Character = Cast<AZSCharacterWithAbilities>(OtherActor);
	UCapsuleComponent * CapsuleComponent = Cast<UCapsuleComponent>(OtherComp);
	if(IsValid(Character) && Character->GetCapsuleComponent() == CapsuleComponent)
	{
		HideVehicleControlWidget();
	}
}

UAbilitySystemComponent* AZSWheeledVehiclePawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AZSWheeledVehiclePawn::MoveForward(float NewValue)
{
	SendForwardInputToServer(NewValue);
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
		SendMoveRightValue(NewValue);
	}
}

void AZSWheeledVehiclePawn::SendMoveRightValue(float NewValue)
{
	if(SteeringInput != NewValue)
	{
		SteeringInput = NewValue;
		Server_SendMoveRightValue(NewValue);
	}
}

void AZSWheeledVehiclePawn::Server_SendMoveRightValue_Implementation(const float & NewValue)
{
	SteeringInput = NewValue;
	USteeringWheelStaticMeshComponent * L_SteeringWheelStaticMeshComponent = GetSteeringWheelStaticMeshComponent();
	if(IsValid(L_SteeringWheelStaticMeshComponent))
	{
		L_SteeringWheelStaticMeshComponent->SetTarget(NewValue);
	}
}

bool AZSWheeledVehiclePawn::Server_SendMoveRightValue_Validate(const float & NewValue)
{
	return !(NewValue > 1 && NewValue < -1);
}


void AZSWheeledVehiclePawn::HandbrakePressed()
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->SetHandbrakeInput(true);
		Server_HandbrakePressed();
	}
}

void AZSWheeledVehiclePawn::Server_HandbrakePressed_Implementation()
{
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.StopLight"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
}

bool AZSWheeledVehiclePawn::Server_HandbrakePressed_Validate()
{
	return true;
}

void AZSWheeledVehiclePawn::HandbrakeReleased()
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->SetHandbrakeInput(false);
		Server_HandbrakeReleased();
	}
}

void AZSWheeledVehiclePawn::Server_HandbrakeReleased_Implementation()
{
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.NoStopLight"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
}

bool AZSWheeledVehiclePawn::Server_HandbrakeReleased_Validate()
{
	return true;
}

void AZSWheeledVehiclePawn::LookUp(float NewValue)
{
	AddControllerPitchInput(NewValue);
}

void AZSWheeledVehiclePawn::LookRight(float NewValue)
{
	AddControllerYawInput(NewValue);
}


void AZSWheeledVehiclePawn::SetupDefaultCamera(ECameraPositionType NewCameraPositionType)
{
	TArray<UZSCameraComponent *> CameraComponents ;
	GetComponents<UZSCameraComponent>(CameraComponents);
	for(UZSCameraComponent * IterCameraComponent : CameraComponents)
	{
		if(IterCameraComponent)
		{
			const ECameraPositionType L_CamPostType = IterCameraComponent->GetCameraPositionType();
			if(L_CamPostType == NewCameraPositionType)
			{
				SelectedCameraPositionType = NewCameraPositionType;
				IterCameraComponent->SetActive(true);
				UpdateSpringLimitationByCameraComponent(IterCameraComponent);
				ShowDriverHead(L_CamPostType);
			}
			else
			{
				IterCameraComponent->SetActive(false);
			}
		}
	}
}

bool AZSWheeledVehiclePawn::IsExistCamera(ECameraPositionType NewCameraPositionType)
{
	TArray<UZSCameraComponent *> CameraComponents ;
	GetComponents<UZSCameraComponent>(CameraComponents);
	for(UZSCameraComponent * IterCameraComponent : CameraComponents)
	{
		if(IterCameraComponent)
		{
			const ECameraPositionType L_CamPostType = IterCameraComponent->GetCameraPositionType();
			if(L_CamPostType == NewCameraPositionType)
			{
				return true;		
			}
		}
	}
	return false;
}

void AZSWheeledVehiclePawn::LeaveVehicle()
{
	Server_LeaveVehicle();
}

void AZSWheeledVehiclePawn::Server_LeaveVehicle_Implementation()
{
	if(ROLE_Authority > GetLocalRole())
	{
		Server_LeaveVehicle();	
	}
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(L_VehicleMovementComponent)
	{
		L_VehicleMovementComponent->StopMovementImmediately();	
	}
	
	if(IsValid(ZSCharacterWithAbilities))
	{
		ZSCharacterWithAbilities->DetachFromVehicle(this);	
	}
}

bool AZSWheeledVehiclePawn::Server_LeaveVehicle_Validate()
{
	return true;
}

void AZSWheeledVehiclePawn::VehicleNextCamera()
{
	const int8 L_CountMax = (static_cast<int8>(ECameraPositionType::CPT_MAX)) - 1;
	for(int8 I = 0; I <= L_CountMax; I++ )
	{
		uint8 L_NextCameraPositionType = static_cast<uint8>(SelectedCameraPositionType);
		const uint8 L_Max = static_cast<uint8>(ECameraPositionType::CPT_MAX);
		L_NextCameraPositionType++;
		if(L_NextCameraPositionType >= L_Max)
		{
			SelectedCameraPositionType = static_cast<ECameraPositionType>(0);
		}
		else
		{
			SelectedCameraPositionType = static_cast<ECameraPositionType>(L_NextCameraPositionType);
		}
		const bool bIsExistCamera = IsExistCamera(SelectedCameraPositionType);
		if(bIsExistCamera)
		{
			break;
		}
	}
	SetupDefaultCamera(SelectedCameraPositionType);
}

void AZSWheeledVehiclePawn::VehicleBackCamera()
{
	const int8 L_Max = (static_cast<int8>(ECameraPositionType::CPT_MAX)) - 1;
	for(int8 I = 0; I <= L_Max; I++ )
	{
		int8 L_BackCameraPositionType = static_cast<int8>(SelectedCameraPositionType);
		L_BackCameraPositionType--;
		if(L_BackCameraPositionType <= 0)
		{
			SelectedCameraPositionType = static_cast<ECameraPositionType>(L_Max);
		}
		else
		{
			SelectedCameraPositionType = static_cast<ECameraPositionType>(L_BackCameraPositionType);
		}
		const bool bIsExistCamera = IsExistCamera(SelectedCameraPositionType);
		if(bIsExistCamera)
		{
			break;
		}
	}
	SetupDefaultCamera(SelectedCameraPositionType);
}

void AZSWheeledVehiclePawn::InitAttributes()
{
	if(IsValid(AbilitySystemComponent))
	{
		AttributeSetVehicle = AbilitySystemComponent->GetSet<UZSVehicleAttributeSet>();
		checkf(nullptr != AttributeSetVehicle, TEXT("The AttributeSetVehicle is nullptr."));
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetHealthBodyAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnHealthBodyChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetHealthEngineAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnHealthEngineChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetGasTankAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnGasTankChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetStopRearLightAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnStopRearLightChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetRearLightAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnRearLightChangedNative);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVehicle->GetFrontAndRearLightAttribute()).AddUObject(this, &AZSWheeledVehiclePawn::OnFrontAndRearLightChangedNative);
	}
}

void AZSWheeledVehiclePawn::GetHealthBody(float& HealthBody, float& MaxHealth)
{
	checkf(nullptr != AttributeSetVehicle, TEXT("The VehicleAttributeSet is nullptr."));
	HealthBody = AttributeSetVehicle->GetHealthBody();
	MaxHealth = AttributeSetVehicle->GetMaxHealthBody();
}

void AZSWheeledVehiclePawn::GetHealthEngine(float& HealthEngine, float& MaxHealthEngine)
{
	checkf(nullptr != AttributeSetVehicle, TEXT("The VehicleAttributeSet is nullptr."));
	HealthEngine = AttributeSetVehicle->GetHealthEngine();
	MaxHealthEngine = AttributeSetVehicle->GetMaxHealthEngine();
}

void AZSWheeledVehiclePawn::GetGasTank(float& GasTank, float & MaxGasTank)
{
	checkf(nullptr != AttributeSetVehicle, TEXT("The VehicleAttributeSet is nullptr."));
	GasTank = AttributeSetVehicle->GetGasTank();
	MaxGasTank = AttributeSetVehicle->GetMaxGasTank();
}

void AZSWheeledVehiclePawn::OnHealthBodyChangedNative(const FOnAttributeChangeData& NewData)
{
	OnHealthBodyChanged(NewData.OldValue, NewData.NewValue);
}

void AZSWheeledVehiclePawn::OnHealthEngineChangedNative(const FOnAttributeChangeData& NewData)
{
	OnHealthEngineChanged(NewData.OldValue, NewData.NewValue);
}

void AZSWheeledVehiclePawn::OnGasTankChangedNative(const FOnAttributeChangeData& NewData)
{
	OnGasTankChanged(NewData.OldValue, NewData.NewValue);
}

void AZSWheeledVehiclePawn::OnStopRearLightChangedNative(const FOnAttributeChangeData& NewData)
{
	OnStopRearLightChanged(NewData.OldValue, NewData.NewValue);
	StopRearLight(NewData);
}

void AZSWheeledVehiclePawn::OnRearLightChangedNative(const FOnAttributeChangeData& NewData)
{
	OnRearLightChanged(NewData.OldValue, NewData.NewValue);
	RearLight(NewData);
}

void AZSWheeledVehiclePawn::OnFrontAndRearLightChangedNative(const FOnAttributeChangeData& NewData)
{
	OnFrontAndRearLightChanged(NewData.OldValue, NewData.NewValue);
	FrontRearLights(NewData);
	const bool bIsEnable = NewData.NewValue != 0 ? true : false;
	OnFrontLights(bIsEnable);
	OnInSideLight(bIsEnable);
}

void AZSWheeledVehiclePawn::StopRearLight(const FOnAttributeChangeData& NewData)
{
	if(IsValid(GetMesh()))
	{
		TArray<USceneComponent *> MeshChildComponent;
		GetMesh()->GetChildrenComponents(true, MeshChildComponent);
		MeshChildComponent.Add(GetMesh());
		for(USceneComponent * IterSceneComponent : MeshChildComponent)
		{
			UPrimitiveComponent * Iter = Cast<UPrimitiveComponent>(IterSceneComponent);
			if(SkipComponent(Iter))
			{
				continue;
			}
			if(IsValid(Iter) )
			{
				const int32 Num = Iter->GetNumMaterials();
				for(int32 I = 0; I < Num; I++)
				{
					UMaterialInstanceDynamic * MatDyn = Cast<UMaterialInstanceDynamic>(Iter->GetMaterial(I));
					if(nullptr == MatDyn)
					{
						MatDyn = Iter->CreateAndSetMaterialInstanceDynamic(I);
					}
					if(MatDyn)
					{
						MatDyn->SetScalarParameterValue(VehicleStopLightParamName, NewData.NewValue);
					}
				}
			}
		}
	}
}

void AZSWheeledVehiclePawn::RearLight(const FOnAttributeChangeData& NewData)
{
	if(IsValid(GetMesh()))
	{
		TArray<USceneComponent *> MeshChildComponent;
		GetMesh()->GetChildrenComponents(true, MeshChildComponent);
		MeshChildComponent.Add(GetMesh());
		for(USceneComponent * IterSceneComponent : MeshChildComponent)
		{
			UPrimitiveComponent * Iter = Cast<UPrimitiveComponent>(IterSceneComponent);
			if(SkipComponent(Iter))
			{
				continue;
			}
			if(IsValid(Iter) )
			{
				const int32 Num = Iter->GetNumMaterials();
				for(int32 I = 0; I < Num; I++)
				{
					UMaterialInstanceDynamic * MatDyn = Cast<UMaterialInstanceDynamic>(Iter->GetMaterial(I));
					if(nullptr == MatDyn)
					{
						MatDyn = Iter->CreateAndSetMaterialInstanceDynamic(I);
					}
					if(MatDyn)
					{
						MatDyn->SetScalarParameterValue(VehicleRearLightParamName, NewData.NewValue);
					}
				}
			}
		}
	}
}

void AZSWheeledVehiclePawn::FrontRearLights(const FOnAttributeChangeData& NewData)
{
	if(IsValid(GetMesh()))
	{
		TArray<USceneComponent *> MeshChildComponent;
		GetMesh()->GetChildrenComponents(true, MeshChildComponent);
		MeshChildComponent.Add(GetMesh());
		for(USceneComponent * IterSceneComponent : MeshChildComponent)
		{
			UPrimitiveComponent * Iter = Cast<UPrimitiveComponent>(IterSceneComponent);
			if(SkipComponent(Iter))
			{
				continue;
			}
			if(IsValid(Iter) )
			{
				const int32 Num = Iter->GetNumMaterials();
				for(int32 I = 0; I < Num; I++)
				{
					UMaterialInstanceDynamic * MatDyn = Cast<UMaterialInstanceDynamic>(Iter->GetMaterial(I));
					if(nullptr == MatDyn)
					{
						MatDyn = Iter->CreateAndSetMaterialInstanceDynamic(I);
					}
					if(MatDyn)
					{
						MatDyn->SetScalarParameterValue(VehicleFrontAndRearLightsParamName, NewData.NewValue);
					}
				}
			}
		}
	}
}

void AZSWheeledVehiclePawn::OnRepHiddenDriver()
{
	if(IsValid(SkeletalMeshComponentDriver))
	{
		SkeletalMeshComponentDriver->SetHiddenInGame(bIsHiddenDriver);
	}
}

void AZSWheeledVehiclePawn::InitializeAbility(TSubclassOf<UGameplayAbility> NewAbilityToGet, int32 AbilityLevel)
{
	if(IsValid(AbilitySystemComponent))
	{
		if(HasAuthority() && IsValid(NewAbilityToGet))
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NewAbilityToGet, AbilityLevel, 0));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AZSWheeledVehiclePawn::CheckVehicleStop()
{
	if(ROLE_Authority == GetLocalRole())
	{
		static bool IsEnableStopRearLight = false;
		static bool IsEnableRearLight = false;
		const FVector Velocity = GetVelocity();
		const FVector Forward = GetActorForwardVector();
		const float Dot = UKismetMathLibrary::Dot_VectorVector(Forward, Velocity);
		if((Dot > 0 && ForwardInput == -1) || (Dot < 0 && ForwardInput == 1)   )
		{
			if( false == IsEnableStopRearLight)
			{
				const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.StopLight"));
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
				IsEnableStopRearLight = true;
			}
		}
		else 
		{   if(IsEnableStopRearLight)
			{
				const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.NoStopLight"));
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
				IsEnableStopRearLight = false;
			}
		}
		if(Dot < 0 && ForwardInput == -1)
		{
			if(false == IsEnableRearLight)
			{
				const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.RearLight"));
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
				IsEnableRearLight = true;
			}
		}
		else
		{
			if(IsEnableRearLight)
			{
				const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.NoRearLight"));
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
				IsEnableRearLight = false;
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("Dot **************** %f, Input = %f ************** "), Dot, ForwardInput);
	}
}

void AZSWheeledVehiclePawn::UnPossessed()
{
	SteeringWheelStaticMeshComponent->SetComponentTickEnabled(false);
	SetIsHiddenDriver(true);
	SetEngineStart(false);
	SetIdleBrakeInput(10);
	//DisableMove();
	MoveForward(0);
	HandbrakePressed();
	if(GetMesh())
	{
		GetMesh()->bOnlyAllowAutonomousTickPose = true;
	}
	Super::UnPossessed();
	TurnOffVehicle();
}

void AZSWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(IsValid(NewController) && !NewController->IsA(AAIController::StaticClass())) // if not AI controller then works below code.
	{
		if(GetMesh())
		{
			GetMesh()->bOnlyAllowAutonomousTickPose = false;
		}
		SetIdleBrakeInput(0);
		SteeringWheelStaticMeshComponent->SetComponentTickEnabled(true);
		SetIsHiddenDriver(false);
		Client_SetupDefaultCamera();
		SetEngineStart(true);
		TurnOnVehicle();
	}
}



void AZSWheeledVehiclePawn::OnFrontLights(const bool & IsEnableLights)
{
	if(ROLE_Authority != GetLocalRole())
	{
		checkf(nullptr != SpotLightComponentFrontLeftLight,  TEXT("The SpotLightComponentFrontLeftLight is nullptr."));
		checkf(nullptr != SpotLightComponentFrontRightLight, TEXT("The SpotLightComponentFrontRightLight is nullptr."));
		if(IsValid(SpotLightComponentFrontLeftLight))
		{
			SpotLightComponentFrontLeftLight->SetVisibility(IsEnableLights);
		}
		if(IsValid(SpotLightComponentFrontRightLight))
		{
			SpotLightComponentFrontRightLight->SetVisibility(IsEnableLights);	
		}
	}
}

void AZSWheeledVehiclePawn::OnInSideLight(const bool& IsEnableLights)
{
	if(IsValid(GetMesh()))
	{
		TArray<USceneComponent *> MeshChildComponent;
		GetMesh()->GetChildrenComponents(true, MeshChildComponent);
		MeshChildComponent.Add(GetMesh());
		for(USceneComponent * IterSceneComponent : MeshChildComponent)
		{
			UPrimitiveComponent * Iter = Cast<UPrimitiveComponent>(IterSceneComponent);
			if(SkipComponent(Iter))
			{
				continue;
			}
			if(IsValid(Iter) )
			{
				const int32 Num = Iter->GetNumMaterials();
				for(int32 I = 0; I < Num; I++)
				{
					UMaterialInstanceDynamic * MatDyn = Cast<UMaterialInstanceDynamic>(Iter->GetMaterial(I));
					if(nullptr == MatDyn)
					{
						MatDyn = Iter->CreateAndSetMaterialInstanceDynamic(I);
					}
					if(MatDyn)
					{
						const float Val = IsEnableLights ? 1.0 : 0;
						MatDyn->SetScalarParameterValue(VehicleInSideLight, Val);
					}
				}
			}
		}
	}
}


void AZSWheeledVehiclePawn::EnableFrontLight()
{
	//UE_LOG(LogTemp, Log, TEXT("******************** The EnableFrontLight *******************"));
	Server_EnableFrontLight();
	
}

void AZSWheeledVehiclePawn::Server_EnableFrontLight_Implementation()
{
	static bool bIsEnable = false;
	if(!bIsEnable)
	{
		const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.EnableFrontAndRearLight"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
		//UE_LOG(LogTemp, Log, TEXT("************* Enable front and Rear Light Server ******************************"));
		bIsEnable = true;
	}
	else
	{
		const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Vehicle.DisableFrontAndRearLight"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, FGameplayEventData());
		//UE_LOG(LogTemp, Log, TEXT("************* Disable front And Rear Light Server ******************************"));
		bIsEnable = false;
	}
}

bool AZSWheeledVehiclePawn::Server_EnableFrontLight_Validate()
{
	return true;
}


USteeringWheelStaticMeshComponent * AZSWheeledVehiclePawn::GetSteeringWheelStaticMeshComponent()
{
	USteeringWheelStaticMeshComponent * L_SteeringWheelStaticMeshComponent = Cast<USteeringWheelStaticMeshComponent>(GetComponentByClass(USteeringWheelStaticMeshComponent::StaticClass()));
	return L_SteeringWheelStaticMeshComponent; 
}

bool AZSWheeledVehiclePawn::SkipComponent(UPrimitiveComponent* NewComponent)
{
	if(Speedometer3D == NewComponent)
	{
		return true;
	}
	if(Cast<UStaticMeshComponent>(NewComponent) ) 
	{
		return false;		
	}
	if(Cast<USkeletalMeshComponent>(NewComponent))
	{
		return false;
	}
	return true;
}

void AZSWheeledVehiclePawn::UpdateSpringLimitationByCameraComponent(UZSCameraComponent* NewCameraComponent)
{
	if(IsValid(NewCameraComponent))
	{
		UZSSpringArmComponent * SpringArmComponent = Cast<UZSSpringArmComponent>(NewCameraComponent->GetAttachParent());
		if(IsValid(SpringArmComponent))
		{
			if(SpringArmComponent->bIsUseControllerInput)
			{
				SpringArmComponent->UpdateLimitation();	
			}
			else
			{
				UZSSpringArmComponent::ResetPlayerCameraManagerRotationLimit(GetWorld());
			}
		}
	}
}

void AZSWheeledVehiclePawn::ShowDriverHead(const ECameraPositionType & NewCameraPositionType)
{
	if( ROLE_AutonomousProxy == GetLocalRole() && IsValid(SkeletalMeshComponentDriver))
	{
			if(ECameraPositionType::CameraInSide == NewCameraPositionType)
			{
				for(const FName & IterName : DriverBoneNameForHHide  )
				{
					SkeletalMeshComponentDriver->HideBoneByName(IterName, EPhysBodyOp::PBO_None);
				}
			}
			else
			{
				for(const FName & IterName : DriverBoneNameForHHide  )
				{
					SkeletalMeshComponentDriver->UnHideBoneByName(IterName);
				}
			}
	}
}

void AZSWheeledVehiclePawn::Client_SetupDefaultCamera_Implementation()
{
	SetupDefaultCamera(SelectedCameraPositionType);
}

void AZSWheeledVehiclePawn::EngineAudioProcessing()
{
	if(ROLE_Authority > GetLocalRole() || UKismetSystemLibrary::IsStandalone(this))
	{
		UChaosWheeledVehicleMovementComponent * VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
		if(IsValid(VehicleMovement))
		{
			const float EngineRPM = VehicleMovement->GetEngineRotationSpeed();
			if(IsValid(EngineSoundComponent))
			{
				EngineSoundComponent->SetFloatParameter(TEXT("RPM"), EngineRPM);
			}
		}
	}
}

void AZSWheeledVehiclePawn::SetDriverSkeletalMesh(AZSCharacterWithAbilities * NewCharacter)
{
	if(IsValid(NewCharacter))
	{
		if(IsValid(NewCharacter->ManageCharacterMeshAC))
		{
			DriverSkeletalMeshName = NewCharacter->MeshName.ToString();
			UE_LOG(LogTemp, Warning, TEXT("--------------------------------- MeshName = %s --------------------------------------"), *DriverSkeletalMeshName);
		}
	}
}

void AZSWheeledVehiclePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION_NOTIFY(AZSWheeledVehiclePawn, SkeletalMeshDriver , COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(AZSWheeledVehiclePawn, bIsHiddenDriver);
	DOREPLIFETIME(AZSWheeledVehiclePawn, DriverSkeletalMeshName);

}

void AZSWheeledVehiclePawn::OnRep_DriverSkeletalMeshName()
{
	SkeletalMeshDriver = GetCharacterSkeletalMeshBySkeletalMeshName(DriverSkeletalMeshName);
	if(IsValid(SkeletalMeshComponentDriver) && IsValid(SkeletalMeshDriver))
	{
		SkeletalMeshComponentDriver->SetSkeletalMesh(SkeletalMeshDriver, true);
		UE_LOG(LogTemp, Warning, TEXT("Client ** ---------------------------------MeshName %s **** --------------------------------------"), *SkeletalMeshDriver->GetName());
		SkeletalMeshComponentDriver->SetHiddenInGame(bIsHiddenDriver);
	}
}

USkeletalMesh* AZSWheeledVehiclePawn::GetCharacterSkeletalMeshBySkeletalMeshName(const FString& NewSkeletalMeshName)
{
	USkeletalMesh * R_SkeletalMesh = nullptr;
	const UZSpaceGameInstance * GameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(this));
	if(IsValid(GameInstance))
	{
		const UCharacterMeshesDataAsset * L_CharacterMeshesDataAsset = GameInstance->GetCharacterMeshesDataAsset();
		if(IsValid(L_CharacterMeshesDataAsset))
		{
			R_SkeletalMesh = L_CharacterMeshesDataAsset->GetMeshByName(FName(NewSkeletalMeshName));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The CharacterMeshesDataAsset is nullptr. Please Set Data asset in GameInstance."));
		}
	}
	return R_SkeletalMesh;
}

void AZSWheeledVehiclePawn::SetIdleBrakeInput_Implementation(float NewInput)
{
	UZSVehicleMovementComponent * L_VehicleMovementComponent = GetZSVehicleMovementComponent();
	if(IsValid(L_VehicleMovementComponent))
	{
		L_VehicleMovementComponent->SetIdleBrakeInput(NewInput);
	}
}


#if WITH_EDITOR
void AZSWheeledVehiclePawn::OnConstruction(const FTransform& MovieSceneBlends)
{
	Super::OnConstruction(MovieSceneBlends);
}
#endif

void AZSWheeledVehiclePawn::EnableMove_Implementation()
{
	USkeletalMeshComponent * L_Mesh = GetMesh();
	if(IsValid(L_Mesh))
	{
		const FName L_BoneName("");
		L_Mesh->SetAllBodiesBelowSimulatePhysics(L_BoneName, true, true);
		L_Mesh->ResetAllBodiesSimulatePhysics();
	}
}

void AZSWheeledVehiclePawn::DisableMove_Implementation()
{
	USkeletalMeshComponent * L_Mesh = GetMesh();
	if(IsValid(L_Mesh))
	{
		const FName L_BoneName("");
		L_Mesh->SetAllBodiesBelowSimulatePhysics(L_BoneName, false, true);
	}
}


bool AZSWheeledVehiclePawn::IsTickWorks() const
{
	bool bIsAIPlayerController = false;
	bIsAIPlayerController = IsValid(Controller) && Controller->IsA(AAIController::StaticClass()) ? true : false;
	return bIsAIPlayerController;
}

void AZSWheeledVehiclePawn::TurnOffVehicle()
{
	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent)
	{
		MovementComponent->SetComponentTickEnabled(false);
	}
	if(GetWorld() && GetWorld()->GetTimerManager().IsTimerPaused(TimerHandleTurnOff))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandleTurnOff);
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandleTurnOff,  TimerDelegateTurnOff, 1, true);
}

void AZSWheeledVehiclePawn::TurnOnVehicle()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
	
	// do not block anything, just ignore
	//SetActorEnableCollision(true);

	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent)
	{
		MovementComponent->SetComponentTickEnabled(true);
	}
	EnableMove();
	
}



void AZSWheeledVehiclePawn::SetEngineStart_Implementation(bool NewValue)
{
	if(ROLE_Authority > GetLocalRole() || UKismetSystemLibrary::IsStandalone(this) )
	{
		bIsEngineStarted = NewValue;
		if (NewValue)
		{
			EngineSoundComponent->SetActive(true);
		}
		else
		{
			EngineSoundComponent->SetActive(false);
		}	
	}
}


void AZSWheeledVehiclePawn::MultiCastEnableTick_Implementation( bool NewEnable)
{
	if(NewEnable)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AZSWheeledVehiclePawn::CanDisableVehicleMovement()
{
	//UE_LOG(LogTemp, Warning, TEXT("************ Called Timer **************** "));
	FVector OutDir(0);
	float OutLength = 0;
	GetVelocity().ToDirectionAndLength(OutDir, OutLength);
	if((IsValid(Controller) && Controller->IsA(AAIController::StaticClass()) || nullptr == Controller) && OutLength < 1   )
	{
		DisableMove();
		GetWorld()->GetTimerManager().PauseTimer(TimerHandleTurnOff);
	}
}

void AZSWheeledVehiclePawn::SwitchToCharacter()
{
	if(IsValid(ZSCharacterWithAbilities))
	{
		AController * PC = GetController();
		if(IsValid(PC))
		{
			DisableMove();
			if(IsValid(ZSCharacterWithAbilities))
			{
				ZSCharacterWithAbilities->IsTransferringBetweenMaps = true;
			}
			SaveVehicleModelOfCharacter();
			PC->Possess(ZSCharacterWithAbilities);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The ZSCharacterWithAbilities is nullptr. "));
	}
}

void AZSWheeledVehiclePawn::SaveVehicleModelOfCharacter()
{
	AZSGamePlayerController * PC = Cast<AZSGamePlayerController>(GetController());
	const APlayerState * L_PlayerState = PC->GetPlayerState<APlayerState>();
	if(IsValid(PC) && IsValid(ZSCharacterWithAbilities) && IsValid(L_PlayerState))
	{
		const UZSpaceGameInstance * GameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(this));
		const FString UserSessionGUID = GameInstance->UserSessionGUID;
		const FString CharacterName = L_PlayerState->GetPlayerName(); 
		if(IsValid(GameInstance))
		{
			const FAssetData L_AssetData = GetClass();
			const FSoftObjectPath L_SoftObjectPath = L_AssetData.ToSoftObjectPath();
			const FString L_VehicleClassName = L_SoftObjectPath.GetAssetPathString();
			PC->AddOrUpdateCosmeticCustomCharacterData(UserSessionGUID, CharacterName, "VehicleName", L_VehicleClassName );
		}
	}
}

void AZSWheeledVehiclePawn::DisableClientCollisions()
{
	if(ROLE_Authority > GetLocalRole())
	{
		TArray<UPrimitiveComponent*> L_Components;
		GetComponents<UPrimitiveComponent>(L_Components);
		for(auto  Iter = L_Components.CreateIterator(); Iter; Iter++)
		{
			UPrimitiveComponent * Iter1 = *Iter;
			if(IsValid(Iter1)
				&& Iter1->IsA(UStaticMeshComponent::StaticClass())
				&& GetMesh() != Iter1) // Skip Root SkeletalMesh component (Vehicle Mesh) 
			{
				Iter1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}
