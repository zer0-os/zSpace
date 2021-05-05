// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "Net/UnrealNetwork.h"

UZSCharacterMovementComponent::UZSCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
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
}

void UZSCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_Authority)
	{
		const float Speed = GetLastUpdateVelocity().Size2D();
		if (Speed == 0.f)
		{
			if (NeedReplicatePlayerGait(EPlayerGait::Standing))
			{
				Server_ChangeMovementMode(EPlayerGait::Standing);
			}
			return;
		}
		if (NeedReplicatePlayerGait(EPlayerGait::Running))
		{
			Server_ChangeMovementMode(EPlayerGait::Running);
		}
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
}

void UZSCharacterMovementComponent::Server_ChangeMovementMode_Implementation(EPlayerGait NewValue)
{
	PlayerGait = NewValue;
	NetMulticast_OnChangedPlayerGait(NewValue);
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
	MaxWalkSpeed = NewValue;
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

void UZSCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZSCharacterMovementComponent, PlayerGait);
}
