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

FName AZSWheeledVehiclePawn::VehicleStopLightParamName = "EmissiveColorStopLights";

FName AZSWheeledVehiclePawn::VehicleRearLightParamName = "EmissiveColorRearLights";

void AZSWheeledVehiclePawn::Server_SetForwardInputValue_Implementation(const float& NewForwardInput)
{
	ForwardInput = NewForwardInput;	
}

bool AZSWheeledVehiclePawn::Server_SetForwardInputValue_Validate(const float& NewForwardInput)
{
	return !(NewForwardInput > 1 || NewForwardInput < -1);
}

void AZSWheeledVehiclePawn::SendForwardInputToServer(const float& NewForwardInput)
{
	if(NewForwardInput != ForwardInput)
	{
		ForwardInput = NewForwardInput;
		Server_SetForwardInputValue(NewForwardInput);
	}
}

AZSWheeledVehiclePawn::AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UZSVehicleMovementComponent>(AWheeledVehiclePawn::VehicleMovementComponentName))
{

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

	CameraComponentDefault = CreateDefaultSubobject<UZSCameraComponent>(TEXT("CameraComponentDefault"));
	checkf(nullptr != CameraComponentDefault, TEXT("The CameraComponentDefault is nullptr."));
	CameraComponentDefault->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponentDefault->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	CameraComponentDefault->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	CameraComponentDefault->bUsePawnControlRotation = false;
	CameraComponentDefault->FieldOfView = 90.f;
	CameraComponentDefault->SetCameraPositionType(ECameraPositionType::DefaultCamera);

	CameraComponentInSide = CreateDefaultSubobject<UZSCameraComponent>(TEXT("CameraComponentInSide"));
	checkf(nullptr != CameraComponentInSide, TEXT("The CameraComponentInSide is nullptr."));
	CameraComponentInSide->SetupAttachment(GetMesh());
	CameraComponentInSide->FieldOfView = 90.f;
	CameraComponentInSide->SetCameraPositionType(ECameraPositionType::CameraInSide);
	

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
	SetAutonomousProxy(true);
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
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(IsValid(PC) && (ROLE_AutonomousProxy == PC->GetLocalRole() || UKismetSystemLibrary::IsStandalone(GetWorld()) ))
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
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(IsValid(PC) && (ROLE_AutonomousProxy == PC->GetLocalRole() || UKismetSystemLibrary::IsStandalone(GetWorld()) ))
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
	// Start an engine sound playing
	EngineSoundComponent->Play(); // TODO Need to set Engine Sound.
	InitAttributes();
	SetActorTickEnabled(false);
}

void AZSWheeledVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(ROLE_Authority == GetLocalRole())
	{
		CheckVehicleStop();
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
		
		PlayerInputComponent->BindAction(TEXT("VehicleNextCamera"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::VehicleNextCamera);
		PlayerInputComponent->BindAction(TEXT("VehicleBackCamera"), EInputEvent::IE_Pressed, this, &AZSWheeledVehiclePawn::VehicleBackCamera);
		
		
	}
}

void AZSWheeledVehiclePawn::ComponentBeginOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(IsValid(PC) && (ROLE_AutonomousProxy == PC->GetLocalRole() || UKismetSystemLibrary::IsStandalone(GetWorld()) ))
	{
		AZSCharacterWithAbilities * Character = Cast<AZSCharacterWithAbilities>(OtherActor);
		UCapsuleComponent * CapsuleComponent = Cast<UCapsuleComponent>(OtherComp);
		if(IsValid(Character) && Character->GetCapsuleComponent() == CapsuleComponent)
		{
			ShowVehicleControlWidget();
		}
	}
}

void AZSWheeledVehiclePawn::ComponentEndOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(IsValid(PC) && (ROLE_AutonomousProxy == PC->GetLocalRole() || UKismetSystemLibrary::IsStandalone(GetWorld()) ))
	{
		AZSCharacterWithAbilities * Character = Cast<AZSCharacterWithAbilities>(OtherActor);
		UCapsuleComponent * CapsuleComponent = Cast<UCapsuleComponent>(OtherComp);
		if(IsValid(Character) && Character->GetCapsuleComponent() == CapsuleComponent)
		{
			HideVehicleControlWidget();
		}
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
	}
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


void AZSWheeledVehiclePawn::LeaveVehicle_Implementation()
{
	if(ROLE_Authority > GetLocalRole())
	{
		LeaveVehicle();	
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

bool AZSWheeledVehiclePawn::LeaveVehicle_Validate()
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
	Super::UnPossessed();
	SetActorTickEnabled(false);
}

void AZSWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetActorTickEnabled(true);
}



