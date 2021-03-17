// Copyright 2018 Sabre Dart Studios


#include "OWSPawnWithAbilities.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/CapsuleComponent.h"

FName AOWSPawnWithAbilities::AbilitySystemComponentName(TEXT("AbilitySystem"));
FName AOWSPawnWithAbilities::MeshComponentName(TEXT("CharacterMesh0"));
FName AOWSPawnWithAbilities::CapsuleComponentName(TEXT("CollisionCylinder"));

// Sets default values
AOWSPawnWithAbilities::AOWSPawnWithAbilities()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(AOWSPawnWithAbilities::AbilitySystemComponentName);
	AbilitySystem->SetIsReplicated(true);

	OWSAttributes = CreateDefaultSubobject<UOWSAttributeSet>(TEXT("AttributeSet"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	OWSPawnMovementComponent = CreateDefaultSubobject<UOWSPawnMovementComponent>(TEXT("OWSPawnMovementComponent"));
	OWSPawnMovementComponent->UpdatedComponent = RootComponent;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacter::MeshComponentName);
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

	BaseRotationOffset = FQuat::Identity;
}

void AOWSPawnWithAbilities::PostInitializeComponents()
{
	//QUICK_SCOPE_CYCLE_COUNTER(STAT_Character_PostInitComponents);

	Super::PostInitializeComponents();

	if (!IsPendingKill())
	{
		if (Mesh)
		{
			CacheInitialMeshOffset(Mesh->GetRelativeLocation(), Mesh->GetRelativeRotation());

			// force animation tick after movement component updates
			if (Mesh->PrimaryComponentTick.bCanEverTick && OWSPawnMovementComponent)
			{
				Mesh->PrimaryComponentTick.AddPrerequisite(OWSPawnMovementComponent, OWSPawnMovementComponent->PrimaryComponentTick);
			}
		}

		if (OWSPawnMovementComponent && CapsuleComponent)
		{
			OWSPawnMovementComponent->UpdateNavAgent(*CapsuleComponent);
		}
	}
}

void AOWSPawnWithAbilities::CacheInitialMeshOffset(FVector MeshRelativeLocation, FRotator MeshRelativeRotation)
{
	BaseTranslationOffset = MeshRelativeLocation;
	BaseRotationOffset = MeshRelativeRotation.Quaternion();

#if ENABLE_NAN_DIAGNOSTIC
	if (BaseRotationOffset.ContainsNaN())
	{
		logOrEnsureNanError(TEXT("ACharacter::PostInitializeComponents detected NaN in BaseRotationOffset! (%s)"), *BaseRotationOffset.ToString());
	}

	const FRotator LocalRotation = Mesh->GetRelativeRotation();
	if (LocalRotation.ContainsNaN())
	{
		logOrEnsureNanError(TEXT("ACharacter::PostInitializeComponents detected NaN in Mesh->RelativeRotation! (%s)"), *LocalRotation.ToString());
	}
#endif
}

// Called when the game starts or when spawned
void AOWSPawnWithAbilities::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOWSPawnWithAbilities::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOWSPawnWithAbilities::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOWSPawnWithAbilities::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Initialize our abilities
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void AOWSPawnWithAbilities::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystem)
	{
		AbilitySystem->RefreshAbilityActorInfo();
	}
}
