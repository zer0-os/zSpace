// Copyright 2020 Sabre Dart Studios


#include "ZSCharacterWithAbilities.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "OculusFunctionLibrary.h"
#include "Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"
#include "Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"
#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "AnimInstances/ZSAnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "OWSGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "zSpace/Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "zSpace/VR/VirtualkeyboarActor/VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"
#include "zSpace/VR/BallisticLineComponent/BallisticLineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ManageCharacterMeshComponent/ManageCharacterMeshAC.h"
#include "zSpace/Game/WheeledVehiclePawn/SpringArmComponent/ZSSpringArmComponent.h"


AZSCharacterWithAbilities::AZSCharacterWithAbilities(const FObjectInitializer& NewObjectInitializer) : Super(NewObjectInitializer.SetDefaultSubobjectClass<UZSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	checkf(nullptr != SpringArmComponent, TEXT("The SpringArmComponent is nullptr."));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->SetRelativeLocation(FVector(0.0,70,68));

	CameraComponentDefault = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	checkf(nullptr != CameraComponentDefault, TEXT("The CameraComponent is nullptr."));
	CameraComponentDefault->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	SceneComponentVR = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentVR"));
	checkf(nullptr != SceneComponentVR, TEXT("The SceneComponentVR is nullptr."));
	SceneComponentVR->SetupAttachment(RootComponent);
	
	CameraComponentVR = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCameraComponent"));
	checkf(nullptr != CameraComponentDefault, TEXT("The VRCameraComponent is nullptr."));
	CameraComponentVR->SetupAttachment(RootComponent);

	DetectSurfaceTypeComponent = CreateDefaultSubobject<UDetectSurfaceTypeComponent>(TEXT("DetectSurfaceTypeComponent"));
	checkf(nullptr != DetectSurfaceTypeComponent, TEXT("The DetectSurfaceTypeComponent is nullptr."));
	AddOwnedComponent(DetectSurfaceTypeComponent);
	
	MotionControllerComponentLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentLeft"));
	checkf(nullptr != MotionControllerComponentLeft, TEXT("The MotionControllerComponentLeft is nullptr."));
	MotionControllerComponentLeft->SetupAttachment(RootComponent);
	MotionControllerComponentLeft->MotionSource = TEXT("Left");
	
	MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentRight"));
	checkf(nullptr != MotionControllerComponentRight, TEXT("The MotionControllerComponentRight is nullptr."));
	MotionControllerComponentRight->SetupAttachment(RootComponent);
	MotionControllerComponentRight->MotionSource = TEXT("Right");

	SkeletalMeshComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentLeft"));
	checkf(nullptr != SkeletalMeshComponentLeft, TEXT("The SkeletalMeshComponentLeft is nullptr.") );
	SkeletalMeshComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	SkeletalMeshComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentRight"));
	checkf(nullptr != SkeletalMeshComponentRight, TEXT("The SkeletalMeshComponentRight is nullptr."));
	SkeletalMeshComponentRight->SetupAttachment(MotionControllerComponentRight);

	WidgetInteractionComponentLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentLeft"));
	checkf(nullptr != WidgetInteractionComponentLeft, TEXT("The WidgetInteractionComponentLeft is nullptr."));
	WidgetInteractionComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	WidgetInteractionComponentLeft->bShowDebug = true;
	WidgetInteractionComponentLeft->InteractionDistance = 1000;
	WidgetInteractionComponentLeft->PointerIndex = 0;
	WidgetInteractionComponentLeft->VirtualUserIndex = 0;
	
	WidgetInteractionComponentRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentRight"));
	checkf(nullptr != WidgetInteractionComponentRight, TEXT("The WidgetInteractionComponentRight is nullptr."));
	WidgetInteractionComponentRight->SetupAttachment(MotionControllerComponentRight);
	WidgetInteractionComponentRight->bShowDebug = true;
	WidgetInteractionComponentRight->InteractionDistance = 1000;
	WidgetInteractionComponentRight->PointerIndex = 1;
	WidgetInteractionComponentRight->VirtualUserIndex = 1;

	WidgetInteractionComponentRight->OnHoveredWidgetChanged.AddUniqueDynamic(this, &AZSCharacterWithAbilities::HoveredWidgetChanged);

	BallisticLineComponentLeft  = CreateDefaultSubobject<UBallisticLineComponent>(TEXT("BallisticLineComponentLeft"));
	checkf(nullptr != BallisticLineComponentLeft, TEXT("The BallisticLineComponentLeft is nullptr.") );
	BallisticLineComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	BallisticLineComponentRight  = CreateDefaultSubobject<UBallisticLineComponent>(TEXT("BallisticLineComponentRight"));
	checkf(nullptr != BallisticLineComponentRight, TEXT("The BallisticLineComponentRight is nullptr.") );
	BallisticLineComponentRight->SetupAttachment(MotionControllerComponentRight);

	ManageCharacterMeshAC = CreateDefaultSubobject<UManageCharacterMeshAC>(TEXT("ManageCharacterMeshAC"));
	checkf(nullptr != ManageCharacterMeshAC, TEXT("The ManageCharacterMeshAC is nullptr. "));
	AddOwnedComponent(ManageCharacterMeshAC);
	
}

void AZSCharacterWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	UZSCharacterMovementComponent* MovementComponent = Cast<UZSCharacterMovementComponent>(GetCharacterMovement());
	if (IsValid(MovementComponent))
	{
		MovementComponent->OnChangedPlayerGait.AddUniqueDynamic(this, &AZSCharacterWithAbilities::OnChangedPlayerGait);
	}
	check(StopMovementAnimMontage);
	check(StartMovementAnimMontage);

	const EResolution& Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	if (Resolution == EResolution::R_5120X1440)
	{
		SpringArmComponent->TargetArmLength = 640.f;
	}
	else if (Resolution != EResolution::None)
	{
		SpringArmComponent->TargetArmLength = 300.f;
	}
	SetupOculusSettings();
}

void AZSCharacterWithAbilities::SetupOculusSettings()
{
	const EOculusDeviceType L_OculusDeviceType = UOculusFunctionLibrary::GetDeviceType();
	if(EOculusDeviceType::Quest_Link == L_OculusDeviceType)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		USkeletalMeshComponent * CharacterSkeletalMeshComponent = GetMesh();
		checkf(nullptr != CharacterSkeletalMeshComponent, TEXT("The CharacterSkeletalmeshComponent is nullptr."));
		//CharacterSkeletalMeshComponent->Deactivate();
		CharacterSkeletalMeshComponent->DestroyComponent();
		CameraComponentDefault->Deactivate();
		CameraComponentVR->Activate();
	} else if(EOculusDeviceType::OculusUnknown == L_OculusDeviceType)
	{
		if(IsValid(MotionControllerComponentLeft))
		{
			MotionControllerComponentLeft->DestroyComponent();
		}
		if(IsValid(MotionControllerComponentRight))
		{
			MotionControllerComponentRight->DestroyComponent();
		}
		if(IsValid(SkeletalMeshComponentLeft))
		{
			SkeletalMeshComponentLeft->DestroyComponent();
		}
		if(IsValid(SkeletalMeshComponentRight))
		{
			SkeletalMeshComponentRight->DestroyComponent();
		}
		if(IsValid(WidgetInteractionComponentLeft))
		{
			WidgetInteractionComponentLeft->DestroyComponent();
		}
		if(IsValid(WidgetInteractionComponentRight))
		{
			WidgetInteractionComponentRight->DestroyComponent();
		}
		if(IsValid(BallisticLineComponentLeft))
		{
			BallisticLineComponentLeft->DestroyComponent();
		}
		if(IsValid(BallisticLineComponentRight ))
		{
			BallisticLineComponentRight->DestroyComponent();
		}
	}
}


void AZSCharacterWithAbilities::SetupPlayerInputComponent(UInputComponent* NewPlayerInputComponent)
{
	if(NewPlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(NewPlayerInputComponent);
		
		NewPlayerInputComponent->BindAxis(TEXT("Turn"), this, &AZSCharacterWithAbilities::Turn);
		NewPlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AZSCharacterWithAbilities::LookUp);
		NewPlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AZSCharacterWithAbilities::MoveForward);
		NewPlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AZSCharacterWithAbilities::MoveRight);
		
		NewPlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AZSCharacterWithAbilities::Jump);
		NewPlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AZSCharacterWithAbilities::StopJumping);
		NewPlayerInputComponent->BindAction(TEXT("ShowPlayersOnline"), IE_Pressed, this, &AZSCharacterWithAbilities::ShowPlayersOnline);
		NewPlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AZSCharacterWithAbilities::Dodge);

		// Walking
		NewPlayerInputComponent->BindAction(TEXT("Walking"), IE_Pressed, this, &AZSCharacterWithAbilities::OnStartWalking);
		NewPlayerInputComponent->BindAction(TEXT("Walking"), IE_Released, this, &AZSCharacterWithAbilities::OnStopWalking);

		// Crouching
		NewPlayerInputComponent->BindAction(TEXT("Crouching"), IE_Pressed, this, &AZSCharacterWithAbilities::OnStartCrouching);
		NewPlayerInputComponent->BindAction(TEXT("Crouching"), IE_Released, this, &AZSCharacterWithAbilities::OnStopCrouching);
		
		NewPlayerInputComponent->BindAction(TEXT("OculusLTeleport"), EInputEvent::IE_Pressed, this, &AZSCharacterWithAbilities::OculusLTeleportPressed);
		NewPlayerInputComponent->BindAction(TEXT("OculusLTeleport"), EInputEvent::IE_Released, this, &AZSCharacterWithAbilities::OculusLTeleportReleased);
		NewPlayerInputComponent->BindAction(TEXT("OculusRTeleport"), EInputEvent::IE_Pressed, this, &AZSCharacterWithAbilities::OculusRTeleportPressed);
		NewPlayerInputComponent->BindAction(TEXT("OculusRTeleport"), EInputEvent::IE_Released, this, &AZSCharacterWithAbilities::OculusRTeleportReleased);
		NewPlayerInputComponent->BindAction(TEXT("EnterVehicle"), EInputEvent::IE_Pressed, this, &AZSCharacterWithAbilities::EnterVehicle);
		
	}
}


void AZSCharacterWithAbilities::Tick(float NewDeltaSeconds)
{
	Super::Tick(NewDeltaSeconds);

	if (HasAuthority())
	{
		const float L_CharacterRelativeRotation = CalculateCharacterRelativeRotation();
		if (L_CharacterRelativeRotation != CharacterRelativeRotation)
		{
			CharacterRelativeRotation = L_CharacterRelativeRotation;
		}

		const float ControlRotationYawAbs = UKismetMathLibrary::NormalizeAxis(GetControlRotation().Yaw);
		const float StartControlRotationYawAbs = StartControlRotationYaw;

		const float Difference = UKismetMathLibrary::Abs(StartControlRotationYawAbs - ControlRotationYawAbs);

		if (Difference > 25.f)
		{
			UZSAnimInstance* AnimInstance = GetAnimInstance();
			if (IsValid(AnimInstance) && IsStartMovementAnimMontagePlaying())
			{
				StopStartMovementAnimMontage(0.25f);
				Server_SetAnimationState(EAnimationState::LoopingInPlaceAnimation);
			}
		}
	}
}

bool AZSCharacterWithAbilities::CanCrouch() const
{
	const bool Result = Super::CanCrouch();

	if (!Result)
	{
		return Result;
	}
	else
	{
		return !GetCharacterMovement()->IsFalling();
	}
}

void AZSCharacterWithAbilities::UnPossessed()
{
	Super::UnPossessed();	
}

void AZSCharacterWithAbilities::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AZSCharacterWithAbilities::Turn(float NewValue)
{
	if(false == IsTransferringBetweenMaps)
	{
		if(GetWorld())
		{
			const float L_DeltaSeconds = GetWorld()->GetDeltaSeconds();
			const float R_Input = NewValue * L_DeltaSeconds * BaseTurnRate;
			AddControllerYawInput(R_Input);
		}
	}
}

void AZSCharacterWithAbilities::LookUp(float NewValue)
{
	if(false == IsTransferringBetweenMaps)
	{
		if(GetWorld())
		{
			const float L_DeltaSeconds = GetWorld()->GetDeltaSeconds();
			const float R_Input = NewValue * L_DeltaSeconds * BaseLookUpRate;
			AddControllerPitchInput(R_Input);
		}
	}
}

void AZSCharacterWithAbilities::JumpIntoWater()
{
	APhysicsVolume * L_PhysicsVolume = GetPawnPhysicsVolume();
	UCharacterMovementComponent * MovementComponent = GetCharacterMovement();	
	if( IsValid(L_PhysicsVolume)
		&& L_PhysicsVolume->bWaterVolume
		&& IsValid(MovementComponent))
	{
		FVector R_Origin;
		FVector R_BoxExtent;
		L_PhysicsVolume->GetActorBounds(false, R_Origin, R_BoxExtent, false);
		R_Origin = (R_BoxExtent.Z * FVector::UpVector) + R_Origin;
		FVector L_ActorLocation = GetActorLocation();
		//UKismetSystemLibrary::DrawDebugSphere(this, L_ActorLocation, 25, 12, FLinearColor::Red, 5);
		//UKismetSystemLibrary::DrawDebugSphere(this, R_Origin, 25, 12, FLinearColor::Black, 5);
		L_ActorLocation.Z += 80;
		if(L_ActorLocation.Z >= R_Origin.Z)
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

void AZSCharacterWithAbilities::Jump()
{
	JumpIntoWater();
	Super::Jump();

	Server_StopMontage(0.25f, AttackMontage);
	StopStopMovementAnimMontage();
	Server_SetAnimationState(EAnimationState::LoopingInPlaceAnimation);
}

void AZSCharacterWithAbilities::StopJumping()
{
	Super::StopJumping();	
}

void AZSCharacterWithAbilities::MoveForward(float NewValue)
{
	if (NewValue != MoveForwardAxisValue)
	{
		Server_SetMoveForwardAxisValue(NewValue);
	}
	MoveForwardAxisValue = NewValue;

	if (MoveForwardAxisValue == 0.f && MoveRightAxisValue == 0.f)
	{
		if (bIsMoveInputPressed)
		{
			Server_SetIsMoveInputPressed(false);
		}
	}
	else
	{
		if (!bIsMoveInputPressed)
		{
			Server_SetIsMoveInputPressed(true);
		}
	}

	const float Value = CalculateMoveInputKeyTimeDownAverage();
	if (Value != MoveInputKeyTimeDownAverage)
	{
		Server_SetMoveInputKeyTimeDownAverage(Value);
	}

	//UE_LOG(LogTemp, Log, TEXT("*************** The NewValue = %f *******************"), NewValue);
	// DataTable'/Game/AbilitySystem/Abilities/MyGameplayTagsTable.MyGameplayTagsTable'
	const FGameplayTag L_GameplayTag = FGameplayTag::RequestGameplayTag(FName("Combat.IsAttackingCannotMove"));
	const bool L_HasMatchingGameplayTag =   AbilitySystem ?  AbilitySystem->HasMatchingGameplayTag(L_GameplayTag) : false;
	if(false == IsTransferringBetweenMaps && false == L_HasMatchingGameplayTag)
	{
		const FQuat L_Quat(FRotator(0,GetControlRotation().Yaw, 0));
		const FVector R_ForwardVector = L_Quat.GetForwardVector();
		AddMovementInput(R_ForwardVector, NewValue);
	}
}

void AZSCharacterWithAbilities::MoveRight(float NewValue)
{
	if (NewValue != MoveRightAxisValue)
	{
		Server_SetMoveRightAxisValue(NewValue);
	}
	MoveRightAxisValue = NewValue;
	
	// DataTable'/Game/AbilitySystem/Abilities/MyGameplayTagsTable.MyGameplayTagsTable'
	const FGameplayTag L_GameplayTag = FGameplayTag::RequestGameplayTag(FName("Combat.IsAttackingCannotMove"));
	const bool L_HasMatchingGameplayTag =   AbilitySystem ?  AbilitySystem->HasMatchingGameplayTag(L_GameplayTag) : false;
	if(false == IsTransferringBetweenMaps && false == L_HasMatchingGameplayTag)
	{
		const FQuat L_Quat(FRotator(0,GetControlRotation().Yaw, 0));
		const FVector R_RightVector = L_Quat.GetRightVector();
		AddMovementInput(R_RightVector, NewValue);
	}
}

void AZSCharacterWithAbilities::Dodge()
{

	const FVector L_Start = GetMesh()->GetSocketLocation(FName("head"));
	FVector L_End = L_Start + GetActorForwardVector() * 500;
	L_End.Z += 500;

	TArray<AActor *> IgnoreActors;
	FHitResult HitResult;

	const bool bIsHit = UKismetSystemLibrary::LineTraceSingle(this, L_Start,  L_End, ETraceTypeQuery(), false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
	
	if(GetOWSMovementComponent() && !bIsHit)
	{
		UZSCharacterMovementComponent* MovementComponent = Cast<UZSCharacterMovementComponent>(GetCharacterMovement());
		bool isAttackMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
			
		if (IsValid(MovementComponent))
		{
			if (false == isAttackMontagePlaying && AnimationState != EAnimationState::Standing && MovementComponent->IsFalling() == false && MovementComponent->MovementMode != EMovementMode::MOVE_Swimming)
			{
				GetOWSMovementComponent()->DoDodge();
						
				if (AnimationState != EAnimationState::StartMovingAnimation)
				{
					USoundBase * L_Acceleration = Cast<USoundBase>(SoundBaseAcceleration.LoadSynchronous());
					checkf(nullptr != L_Acceleration, TEXT("The L_Acceleration is nullptr., Pleas Set Acceleration Sound."));
					if(L_Acceleration)
					{
						UGameplayStatics::PlaySoundAtLocation(this, L_Acceleration, GetActorLocation());
					}
				}
			}
		}
	}
}

void AZSCharacterWithAbilities::ShowPlayersOnline()
{
	Server_ShowPlayersOnline();
}

void AZSCharacterWithAbilities::OnStartWalking()
{
	Server_SetIsWalking(true);
}

void AZSCharacterWithAbilities::OnStopWalking()
{
	Server_SetIsWalking(false);
}

void AZSCharacterWithAbilities::OnStartCrouching()
{
	if (CanCrouch())
	{
		Crouch();
		Server_StopMontage(0.25f, AttackMontage);
	}
}

void AZSCharacterWithAbilities::OnStopCrouching()
{
	UnCrouch();
}

void AZSCharacterWithAbilities::Server_ShowPlayersOnline_Implementation()
{
	AOWSGameMode * L_GameMode = GetGameMode();
	if(L_GameMode)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Characters Online:"));
		for(const FCharactersOnlineStruct & Iter :  L_GameMode->CharactersOnline)
		{
			const FString L_Log = FString::Printf(TEXT("(Character Name %s ) : (Zone Name %s )"), *Iter.CharName, *Iter.ZoneName);
		}
	}
}

bool AZSCharacterWithAbilities::Server_ShowPlayersOnline_Validate()
{
	return true;
}

void AZSCharacterWithAbilities::Server_SetIsWalking_Implementation(bool NewValue)
{
	bIsWalking = NewValue;
}

bool AZSCharacterWithAbilities::Server_SetIsWalking_Validate(bool NewValue)
{
	return true;
}

void AZSCharacterWithAbilities::Server_SetIsMoveInputPressed_Implementation(bool NewValue)
{
	bIsMoveInputPressed = NewValue;
	if (NewValue)
	{
		StopStopMovementAnimMontage();
	}
}

bool AZSCharacterWithAbilities::Server_SetIsMoveInputPressed_Validate(bool NewValue)
{
	return true;
}

void AZSCharacterWithAbilities::Server_SetMoveForwardAxisValue_Implementation(const float& NewValue)
{
	MoveForwardAxisValue = NewValue;
}

bool AZSCharacterWithAbilities::Server_SetMoveForwardAxisValue_Validate(const float& NewValue)
{
	return true;
}

void AZSCharacterWithAbilities::Server_SetMoveRightAxisValue_Implementation(const float& NewValue)
{
	MoveRightAxisValue = NewValue;
}

bool AZSCharacterWithAbilities::Server_SetMoveRightAxisValue_Validate(const float& NewValue)
{
	return  true;
}

void AZSCharacterWithAbilities::Server_SetMoveInputKeyTimeDownAverage_Implementation(const float& NewValue)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	
	if (bIsMoveInputPressed && NewValue != 0.f)
	{
		MoveInputKeyTimeDownAverage = NewValue;
	}

	if (bIsMoveInputPressed)
	{
		TimerManager.SetTimer(MoveInputKeyTimeDownAverage_TimerHandle, [this]()
		{
			MoveInputKeyTimeDownAverage = 0.f;
		}, 0.5f, false, 0.5f);
	}
}

bool AZSCharacterWithAbilities::Server_SetMoveInputKeyTimeDownAverage_Validate(const float& NewValue)
{
	return true;
}

void AZSCharacterWithAbilities::Server_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
	FName StartSectionName, bool PlayInServer)
{
	if (!IsValid(AnimMontage)) return;
	NetMulticast_PlayMontage(AnimMontage, InPlayRate, StartSectionName, PlayInServer);
}

bool AZSCharacterWithAbilities::Server_PlayMontage_Validate(UAnimMontage* AnimMontage, float InPlayRate,
	FName StartSectionName, bool PlayInServer)
{
	return true;
}

void AZSCharacterWithAbilities::NetMulticast_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
	FName StartSectionName, bool PlayInServer)
{
	if (!IsValid(AnimMontage)) return;
	if (HasAuthority() && !PlayInServer) return;
		
	PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void AZSCharacterWithAbilities::OnRep_AnimationState()
{
	
}

bool AZSCharacterWithAbilities::IsStopMovementAnimMontagePlaying() const
{
	if(!IsValid(GetMesh())) return false;
	
	UZSAnimInstance* AnimInstance = Cast<UZSAnimInstance>(GetMesh()->GetAnimInstance());
	if(!IsValid(AnimInstance)) return false;

	if (!IsValid(StopMovementAnimMontage)) return false;

	for (auto* Montage : StopMovementAnimMontage->GetAllMontages())
	{
		const bool& IsPlaying = AnimInstance->Montage_IsPlaying(Montage);
		if (IsPlaying)
		{
			return true;
		}
	}

	return false;
}

bool AZSCharacterWithAbilities::IsStartMovementAnimMontagePlaying() const
{
	if(!IsValid(GetMesh())) return false;
	
	UZSAnimInstance* AnimInstance = Cast<UZSAnimInstance>(GetMesh()->GetAnimInstance());
	if(!IsValid(AnimInstance)) return false;

	if (!IsValid(StartMovementAnimMontage)) return false;
	
	for (auto* Montage : StartMovementAnimMontage->GetAllMontages())
	{
		const bool& IsPlaying = AnimInstance->Montage_IsPlaying(Montage);
		if (IsPlaying)
		{
			return true;
		}
	}

	return false;
}

void AZSCharacterWithAbilities::Server_StopMontage_Implementation(float InBlendOutTime, const UAnimMontage* Montage)
{
	if (!IsValid(Montage)) return;
	NetMulticast_StopMontage(InBlendOutTime, Montage);
}

bool AZSCharacterWithAbilities::Server_StopMontage_Validate(float InBlendOutTime, const UAnimMontage* Montage)
{
	return true;
}

void AZSCharacterWithAbilities::NetMulticast_StopMontage_Implementation(float InBlendOutTime,
	const UAnimMontage* Montage)
{
	if (!IsValid(GetMesh())) return;

	UZSAnimInstance* AnimInstance = Cast<UZSAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Stop(InBlendOutTime, Montage);
	}
}

UAnimMontage* AZSCharacterWithAbilities::PlayStopMovementAnimMontage()
{
	UZSCharacterMovementComponent* L_CharacterMovementComponent = GetZSCharacterMovement();
	if(!IsValid(L_CharacterMovementComponent)) return nullptr;

	if (L_CharacterMovementComponent->IsFalling()) return nullptr;
	
	if (!IsValid(StopMovementAnimMontage)) return nullptr;
	if (IsStopMovementAnimMontagePlaying()) return nullptr;
	
	if(!IsValid(GetMesh())) return nullptr;
	
	UZSAnimInstance* AnimInstance = Cast<UZSAnimInstance>(GetMesh()->GetAnimInstance());
	if(!IsValid(AnimInstance)) return nullptr;

	const ECharacterFootType CharacterFoot = AnimInstance->GetCharacterFoot();
	const EPlayerGait PlayerGaitPreStanding = L_CharacterMovementComponent->GetPlayerGaitPreStanding();

	UAnimMontage* Montage = StopMovementAnimMontage->GetAnimMontageByGaitAndFoot(PlayerGaitPreStanding, CharacterFoot);

	if (!IsValid(Montage)) return nullptr;
	
	if (HasAuthority())
	{
		NetMulticast_PlayMontage(Montage, 1.f, NAME_None, true);
		return Montage;
	}
	else if (IsLocallyControlled())
	{
		Server_PlayMontage(Montage, 1.f, NAME_None, true);
		return Montage;
	}

	return nullptr;
}

UAnimMontage* AZSCharacterWithAbilities::PlayStartMovementAnimMontage()
{
	UZSCharacterMovementComponent* L_CharacterMovementComponent = GetZSCharacterMovement();
	if(!IsValid(L_CharacterMovementComponent)) return nullptr;

	if (L_CharacterMovementComponent->IsFalling()) return nullptr;
	
	if (!(GetIsMoveInputPressed() && AnimationState == EAnimationState::Standing)) return nullptr;

	if (!IsValid(StartMovementAnimMontage)) return nullptr;
	if (IsStartMovementAnimMontagePlaying()) return nullptr;
	
	if(!IsValid(GetMesh())) return nullptr;
	
	UZSAnimInstance* AnimInstance = Cast<UZSAnimInstance>(GetMesh()->GetAnimInstance());
	if(!IsValid(AnimInstance)) return nullptr;

	const EPlayerGait PlayerGait = L_CharacterMovementComponent->GetPlayerGait();
	const EPlayerMoveDirection& PlayerMoveDirection = AnimInstance->CalculateStartMoveDirection();
	
	UAnimMontage* Montage = StartMovementAnimMontage->GetAnimMontageByGaitAndDirection(PlayerGait, PlayerMoveDirection);
	
	if (!IsValid(Montage)) return nullptr;
	
	Server_SetAnimationState(EAnimationState::StartMovingAnimation);
	
	if (HasAuthority())
	{
		NetMulticast_PlayMontage(Montage, 1.f, NAME_None, true);
		StartControlRotationYaw = UKismetMathLibrary::NormalizeAxis(GetControlRotation().Yaw);
		
		return Montage;
	}
	else if (IsLocallyControlled())
	{
		Server_PlayMontage(Montage, 1.f, NAME_None, true);
		return Montage;
	}
	
	return nullptr;
}

void AZSCharacterWithAbilities::StopStopMovementAnimMontage()
{
	for (auto *Montage : StopMovementAnimMontage->GetAllMontages())
	{
		if (!IsValid(Montage)) return;
		
		if (HasAuthority())
		{
			NetMulticast_StopMontage(0.25f, Montage);
		}
		else if (IsLocallyControlled())
		{
			Server_StopMontage(0.25f, Montage);
		}
	}
}

void AZSCharacterWithAbilities::StopStartMovementAnimMontage(float InBlendOutTime/*=0.25*/)
{
	for (auto *Montage : StartMovementAnimMontage->GetAllMontages())
	{
		if (!IsValid(Montage)) return;
		
		if (HasAuthority())
		{
			NetMulticast_StopMontage(InBlendOutTime, Montage);
		}
		else if (IsLocallyControlled())
		{
			Server_StopMontage(InBlendOutTime, Montage);
		}
	}
}

UZSCharacterMovementComponent* AZSCharacterWithAbilities::GetZSCharacterMovement() const
{
	return Cast<UZSCharacterMovementComponent>(GetCharacterMovement());
}

UZSAnimInstance* AZSCharacterWithAbilities::GetAnimInstance() const
{
	USkeletalMeshComponent* L_Mesh = GetMesh();
	if (IsValid(L_Mesh))
	{
		return Cast<UZSAnimInstance>(L_Mesh->GetAnimInstance());
	}
	
	return nullptr;
}

void AZSCharacterWithAbilities::NetMulticast_OnChangeAnimationState_Implementation(const EAnimationState& CurrentValue)
{
	OnChangeAnimationState.Broadcast(CurrentValue);
}

void AZSCharacterWithAbilities::Server_SetAnimationState_Implementation(const EAnimationState& NewValue)
{
	AnimationState = NewValue;
	NetMulticast_OnChangeAnimationState(NewValue);
}

bool AZSCharacterWithAbilities::Server_SetAnimationState_Validate(const EAnimationState& NewValue)
{
	return true;
}

void AZSCharacterWithAbilities::OnChangedPlayerGait(EPlayerGait CurrentPlayerGait)
{
	if(CurrentPlayerGait == EPlayerGait::Standing)
	{
		if (HasAuthority())
		{
			if (MoveInputKeyTimeDownAverage > 0.04f && AnimationState == EAnimationState::LoopingInPlaceAnimation)
			{
				PlayStopMovementAnimMontage();
			}
			else
			{
				StopStartMovementAnimMontage(0.25f);
			}
		}
	}
}

float AZSCharacterWithAbilities::CalculateCharacterRelativeRotation() const
{
	const FRotator& ActorRotation = GetActorRotation();
	const FRotator& ControlRotation = GetControlRotation();

	const FTransform A = {ActorRotation.Quaternion(), FVector::ZeroVector};
	const FTransform B = {ControlRotation.Quaternion(), FVector::ZeroVector};

	const FTransform& Result = A.GetRelativeTransform(B);

	return Result.Rotator().Yaw * -1.f;
}

float AZSCharacterWithAbilities::CalculateMoveInputKeyTimeDownAverage() const
{
	float Result = 0.f;
	
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!IsValid(InputSettings)) return Result;

	TArray<FInputAxisKeyMapping> OutMappings;
	InputSettings->GetAxisMappingByName(FName("MoveRight"), OutMappings);
	InputSettings->GetAxisMappingByName(FName("MoveForward"), OutMappings);

	APlayerController* PC = GetController<APlayerController>();
	if (!IsValid(PC)) return Result;

	float DownTimes = 0.f;
	for (auto Input : OutMappings)
	{
		const FKey& Key = Input.Key;
		const float DownTime = PC->GetInputKeyTimeDown(Key);
		DownTimes += DownTime;
	}

	Result = DownTimes / 4.f;
	
	return Result;
}

void AZSCharacterWithAbilities::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZSCharacterWithAbilities, bIsDeath);
	DOREPLIFETIME(AZSCharacterWithAbilities, bIsWalking);
	DOREPLIFETIME(AZSCharacterWithAbilities, AnimationState);
	DOREPLIFETIME(AZSCharacterWithAbilities, bIsMoveInputPressed);
	DOREPLIFETIME(AZSCharacterWithAbilities, CharacterRelativeRotation);
	DOREPLIFETIME(AZSCharacterWithAbilities, MoveInputKeyTimeDownAverage);
	DOREPLIFETIME(AZSCharacterWithAbilities, MeshName);

	// Move Axis Values
	DOREPLIFETIME(AZSCharacterWithAbilities, MoveForwardAxisValue);
	DOREPLIFETIME(AZSCharacterWithAbilities, MoveRightAxisValue);
}

void AZSCharacterWithAbilities::OculusLTeleportPressed()
{
	BallisticLineComponentLeft->ShowBallisticLine();
}

void AZSCharacterWithAbilities::OculusLTeleportReleased()
{
	BallisticLineComponentLeft->HideBallisticLine();
}

void AZSCharacterWithAbilities::OculusRTeleportPressed()
{
	BallisticLineComponentRight->ShowBallisticLine();
}

void AZSCharacterWithAbilities::OculusRTeleportReleased()
{
	BallisticLineComponentRight->HideBallisticLine();
}

void AZSCharacterWithAbilities::HoveredWidgetChanged(UWidgetComponent* NewWidgetComponent, UWidgetComponent* NewPreviousWidgetComponent)
{
	if(IsValid(NewWidgetComponent) && IsValid(WidgetInteractionComponentRight))
	{
		UUserWidget * L_UserWidgetObject = NewWidgetComponent->GetUserWidgetObject();
		if(IsValid(L_UserWidgetObject))
		{
			const bool L_IsImplemented = L_UserWidgetObject->GetClass()->ImplementsInterface(UVirtualKeyboardWidgetInterface::StaticClass());
			if(L_IsImplemented)
			{
				IVirtualKeyboardWidgetInterface::Execute_SetWidgetInteractionComponent(L_UserWidgetObject, WidgetInteractionComponentRight);	
			}
		}
	}
}


void AZSCharacterWithAbilities::EnterVehicle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("********************************** Enter Vehicle ***********************"));
	if(ROLE_Authority > GetLocalRole())
	{
		UE_LOG(LogTemp, Log, TEXT("Client: Enger Vehicle "));
		EnterVehicle();	
	}
	if(!IsValid(GetOWSPlayerController()))
	{
		return;	
	}
	UE_LOG(LogTemp, Log, TEXT("Server: Enger Vehicle "));
	TArray<UPrimitiveComponent *> OverlappingComponents;
	GetOverlappingComponents(OverlappingComponents);
	for( UPrimitiveComponent * Iter : OverlappingComponents)
	{
		UBoxComponent * IterBoxComponent = Cast<UBoxComponent>(Iter);
		if(IsValid(IterBoxComponent))
		{
			AZSWheeledVehiclePawn * Vehicle = Cast<AZSWheeledVehiclePawn>(IterBoxComponent->GetOwner());
			if(IsValid(IterBoxComponent) && nullptr != Vehicle )
			{
				AttachToVehicle(Vehicle);
			}
			
		}
	}
}

bool AZSCharacterWithAbilities::EnterVehicle_Validate()
{
	return true;
}

void AZSCharacterWithAbilities::AttachToVehicle(AZSWheeledVehiclePawn * NewVehicle)
{
	if(ROLE_Authority == GetLocalRole())
	{
		if(IsValid(NewVehicle) && NewVehicle->IsEnterVehicle(this))
		{
			SetActorEnableCollision(false);
			AttachToActor(NewVehicle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			const FVector L_VehicleLocation = NewVehicle->GetActorLocation();
			UCapsuleComponent * L_CapsuleComponent = GetCapsuleComponent();
			checkf( nullptr != L_CapsuleComponent, TEXT("The CapsuleComponent is nullptr."));
			SetActorLocation(L_VehicleLocation);
			SetActorHiddenInGame(true);
			AOWSPlayerController * PC = GetOWSPlayerController();
			if(IsValid(PC))
			{
				Client_AttachToVehicle(NewVehicle);
				PC->Possess(NewVehicle);
				NewVehicle->SetDriverSkeletalMesh(this);
				NewVehicle->SetZsCharacterWithAbilities(this);
				UE_LOG(LogTemp, Log, TEXT("Server: Posses"));
			}
		}
	}
}

void AZSCharacterWithAbilities::Client_AttachToVehicle_Implementation(AZSWheeledVehiclePawn* NewVehicle)
{
	if(IsValid(NewVehicle))
	{
		SetActorEnableCollision(false);
		//AttachToActor(NewVehicle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//const FVector L_VehicleLocation = NewVehicle->GetActorLocation();
		//UCapsuleComponent * L_CapsuleComponent = GetCapsuleComponent();
		//checkf( nullptr != L_CapsuleComponent, TEXT("The CapsuleComponent is nullptr."));
		///SetActorLocation(L_VehicleLocation);
		SetActorHiddenInGame(true);
	}
}


void AZSCharacterWithAbilities::DetachFromVehicle(AZSWheeledVehiclePawn* NewVehicle)
{
	if(ROLE_Authority == GetLocalRole())
	{
		if(IsValid(NewVehicle))
		{
			bool bLeaveAvailable = false;
			const FVector CharacterLocation = GetPossibleLeaveCarLocation(NewVehicle, bLeaveAvailable);
			if(bLeaveAvailable)
			{
				APlayerController * PC = Cast<APlayerController>(NewVehicle->GetController());
				if(PC)
				{
					constexpr EDetachmentRule LocationRule = EDetachmentRule::KeepRelative;
					constexpr EDetachmentRule RotationRule = EDetachmentRule::KeepRelative;
					constexpr EDetachmentRule ScaleRule = EDetachmentRule::KeepRelative;
					DetachFromActor(FDetachmentTransformRules(LocationRule, RotationRule, ScaleRule, true));
					SetActorRotation(FRotator(0, 0, 0), ETeleportType::ResetPhysics);
					SetActorLocation(CharacterLocation);
					SetActorEnableCollision(true);
					SetActorHiddenInGame(false);
					PC->Possess(this);
					Client_DetachFromVehicle();
					const FVector VehicleForwardVector = GetActorForwardVector();
					NewVehicle->SetZsCharacterWithAbilities(nullptr);
				}
			}
		}
	}
}

void AZSCharacterWithAbilities::Client_DetachFromVehicle_Implementation()
{
	//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorRotation(FRotator(0, 0, 0), ETeleportType::ResetPhysics);
	UZSSpringArmComponent::ResetPlayerCameraManagerRotationLimit(GetWorld());
}

FVector AZSCharacterWithAbilities::GetPossibleLeaveCarLocation(AZSWheeledVehiclePawn* NewVehicle, bool & NewStatus)
{
	checkf(nullptr != NewVehicle, TEXT("The NewVehicle is nullptr."));
	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(NewVehicle->GetCharacterWithAbilities());	
	TArray<FHitResult> OutHits;
	const FVector VehicleRightVector =  NewVehicle->GetActorRightVector();
	const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	FVector Start = NewVehicle->GetActorLocation();
	constexpr float L_LengthLeaveDirection = 300.0f;
	float L_LengthLevelDirectionFinal = EVehicleLiveDirection::LEFT_DIRECTION == NewVehicle->VehicleLiveDirection ? -L_LengthLeaveDirection : L_LengthLeaveDirection;
	FVector End = NewVehicle->GetActorLocation() + (VehicleRightVector * L_LengthLevelDirectionFinal);
	End.Z += 150.f;
	const bool bIsHit = UKismetSystemLibrary::LineTraceMulti(NewVehicle, Start, End, TraceTypeQuery,true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true);
	if(!bIsHit)
	{
		NewStatus = true;
		return End;
	}
	L_LengthLevelDirectionFinal = EVehicleLiveDirection::RIGHT_DIRECTION == NewVehicle->VehicleLiveDirection ? -L_LengthLeaveDirection : L_LengthLeaveDirection;
	End = NewVehicle->GetActorLocation() + (VehicleRightVector * L_LengthLevelDirectionFinal);
	const bool bIsHit1 = UKismetSystemLibrary::LineTraceMulti(NewVehicle, Start, End, TraceTypeQuery,true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true);
	if(!bIsHit1)
	{
		NewStatus = true;
		return End;
	}
	NewStatus = false;
	UE_LOG(LogTemp, Warning, TEXT("The Character deosn't leave from vehicle becouse not available location"));
	return FVector::ZeroVector;	
	
}

void AZSCharacterWithAbilities::ShowEnterVehicleWidget_Implementation(AZSWheeledVehiclePawn* NewVehicle)
{
	if(IsValid(NewVehicle))
	{
		NewVehicle->ShowVehicleControlWidget();
	}
}



