// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"

#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "Net/UnrealNetwork.h"
#include "zSpace/zSpace.h"

UZSCharacterMovementComponent::UZSCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	GetNavAgentPropertiesRef().bCanCrouch = true;
}

void UZSCharacterMovementComponent::OnMovementModeChanged(EMovementMode NewPreviousMovementMode, uint8 NewPreviousCustomMode)
{
	//FString L_CharacterMode = UEnum::GetValueAsString<EMovementMode>(PreviousMovementMode);
	//UE_LOG(LogTemp, Warning, TEXT("************* PreviousMovementMode = %s, PreviousCustomMode = %d"), *L_CharacterMode, PreviousCustomMode);
	OnMovementModeChangedDelegate.Broadcast(NewPreviousMovementMode, MovementMode, NewPreviousCustomMode);
	Super::OnMovementModeChanged(NewPreviousMovementMode, NewPreviousCustomMode);
}

void UZSCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetAnimInstance();
	OwnerZSCharacter = Cast<AZSCharacterWithAbilities>(GetOwner());
}

void UZSCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwnerZSCharacter)) return;;

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (OwnerZSCharacter->IsStopMovementAnimMontagePlaying())
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Standing))
			{
				Server_ChangeMovementMode(EPlayerGait::Standing);
			}
			return;
		}
		
		// Standing
		const float Speed = OwnerZSCharacter->GetVelocity().Size2D();
		if (Speed == 0.f || !OwnerZSCharacter->bIsMoveInputPressed)
		// if (!OwnerZSCharacter->bIsMoveInputPressed)
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Standing))
			{
				Server_ChangeMovementMode(EPlayerGait::Standing);
			}
			return;
		}
		// Sprinting
		if (bRequestToStartSprinting)
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Sprinting))
			{
				Server_ChangeMovementMode(EPlayerGait::Sprinting);
			}
		}
		// Walking
		else if (OwnerZSCharacter->bIsWalking)
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Walking))
			{
				Server_ChangeMovementMode(EPlayerGait::Walking);
			}
		}
		// Running
		else if (!IsCrouching())
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Running))
			{
				Server_ChangeMovementMode(EPlayerGait::Running);
			}
		}
		if (IsCrouching())
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Crouching))
			{
				Server_ChangeMovementMode(EPlayerGait::Crouching);
			}
		}
		// Update Max Walk Speed
		UpdateMaxWalkSpeed();
	}
}

float UZSCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = UCharacterMovementComponent::GetMaxSpeed();

	return MaxSpeed;
}

void UZSCharacterMovementComponent::Server_ChangeMovementMode_Implementation(EPlayerGait NewValue)
{
	PlayerGait = NewValue;
	NetMulticast_OnChangedPlayerGait(NewValue);
	if (NewValue != EPlayerGait::Standing)
	{
		PlayerGaitPreStanding = NewValue;
	}
}

bool UZSCharacterMovementComponent::Server_ChangeMovementMode_Validate(EPlayerGait NewValue)
{
	return true;
}

void UZSCharacterMovementComponent::NetMulticast_OnChangedPlayerGait_Implementation(EPlayerGait NewValue)
{
	OnChangedPlayerGait.Broadcast(NewValue);
}

void UZSCharacterMovementComponent::NetMulticast_SetMaxWalkSpeed_Implementation(const float& NewValue)
{
	if (IsCrouching())
	{
		MaxWalkSpeedCrouched = NewValue;
	}
	else
	{
		MaxWalkSpeed = NewValue;
	}
}

UAnimInstance* UZSCharacterMovementComponent::GetAnimInstance() const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
	{
		USkeletalMeshComponent* Mesh = Character->GetMesh();
		if (IsValid(Mesh))
		{
			return Mesh->GetAnimInstance();
		}
	}

	return nullptr;
}

void UZSCharacterMovementComponent::UpdateMaxWalkSpeed()
{
	if (IsValid(AnimInstance))
	{
		float SpeedCurveValue;
		if (AnimInstance->GetCurveValue(FName("Speed"), SpeedCurveValue))
		{
			if (NeedReplicateMaxWalkSpeed(SpeedCurveValue))
			{
				NetMulticast_SetMaxWalkSpeed(SpeedCurveValue);
			}
		}
	}	
}

void UZSCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZSCharacterMovementComponent, PlayerGait);
	DOREPLIFETIME(UZSCharacterMovementComponent, PlayerGaitPreStanding);
}
