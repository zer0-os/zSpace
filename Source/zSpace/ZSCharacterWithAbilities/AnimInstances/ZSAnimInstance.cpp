// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimInstance.h"

#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"
#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/zSpace.h"

void UZSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CharacterRef = Cast<AZSCharacterWithAbilities>(GetOwningActor());
	if (IsValid(CharacterRef))
	{
		CharacterMovementComponent = Cast<UZSCharacterMovementComponent>(CharacterRef->GetCharacterMovement());
		if (IsValid(CharacterMovementComponent))
		{
			CharacterMovementComponent->OnChangedPlayerGait.AddUniqueDynamic(this, &UZSAnimInstance::OnChangedPlayerGait);
		}
	}
}

void UZSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(CharacterRef)) return;
	if (!IsValid(CharacterMovementComponent)) return;

	// Speed
	Speed = CharacterRef->GetVelocity().Size2D();
	// Direction [-180 180]
	Direction = CalculateDirection(CharacterRef->GetVelocity(), CharacterRef->GetActorRotation());
	// Air
	bIsInAir = CharacterMovementComponent->IsFalling();
	// Crouching
	bIsCrouching = CharacterMovementComponent->IsCrouching();

	// Move Input Pressed
	bIsMoveInputPressed = CharacterRef->GetIsMoveInputPressed();

	// Axis Value
	MoveForwardAxisValue = CharacterRef->GetMoveForwardAxisValue();
	LastMoveForwardAxisValue = CharacterRef->GetLastMoveForwardAxisValue();
	MoveRightAxisValue = CharacterRef->GetMoveRightAxisValue();
	LastMoveRightAxisValue = CharacterRef->GetLastMoveRightAxisValue();

	bIsMovingOnlyForward = IsMovingOnlyForward();

	PlayerMoveDirection = CalculatePlayerMoveDirection();
	LastPlayerMoveDirection = LastCalculatePlayerMoveDirection();
}

void UZSAnimInstance::OnChangedPlayerGait(EPlayerGait NewValue)
{
	PlayerGait = NewValue;
}

bool UZSAnimInstance::CurveIsActive(const EAnimCurveType& AnimCurveType, const FName& CurveName) const
{
	TArray<FName> OutNames;
	GetActiveCurveNames(AnimCurveType, OutNames);
	const int32 Index = OutNames.Find(CurveName);

	return OutNames.IsValidIndex(Index);
}

bool UZSAnimInstance::IsMovingOnlyForward() const
{
	return MoveForwardAxisValue > 0.f && MoveRightAxisValue == 0.f;
}

bool UZSAnimInstance::IsMovedOnlyForward() const
{
	return LastMoveForwardAxisValue > 0.1f && LastMoveRightAxisValue == 0.f;
}

EPlayerMoveDirection UZSAnimInstance::CalculatePlayerMoveDirection() const
{
	// Forward
	if (MoveForwardAxisValue > 0.f && MoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::Forward;	
	}

	// Backward
	if (MoveForwardAxisValue < 0.f && MoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::Backward;
	}
	
	// Right Or Left
	if (MoveForwardAxisValue == 0.f && MoveRightAxisValue != 0.f)
	{
		return MoveRightAxisValue > 0.f ? EPlayerMoveDirection::Right : EPlayerMoveDirection::Left;
	}

	if (MoveForwardAxisValue != 0.f && MoveRightAxisValue != 0.f)
	{
		// Right Forward
		if (MoveForwardAxisValue > 0.f && MoveRightAxisValue > 0.f)
		{
			return EPlayerMoveDirection::RightForward;
		}
		// Left Forward
		if (MoveForwardAxisValue > 0.f && MoveRightAxisValue < 0.f)
		{
			return EPlayerMoveDirection::LeftForward;
		}
		// Right Backward
		if (MoveForwardAxisValue < 0.f && MoveRightAxisValue > 0.f)
		{
			return EPlayerMoveDirection::RightBackward;
		}
		// Left Backward
		if (MoveForwardAxisValue < 0.f && MoveRightAxisValue < 0.f)
		{
			return EPlayerMoveDirection::LeftBackward;
		}
	}
	
	return EPlayerMoveDirection::Forward;	
}

EPlayerMoveDirection UZSAnimInstance::LastCalculatePlayerMoveDirection() const
{
	// Forward
	if (LastMoveForwardAxisValue > 0.f && LastMoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::Forward;	
	}

	// Backward
	if (LastMoveForwardAxisValue < 0.f && LastMoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::Backward;
	}
	
	// Right Or Left
	if (LastMoveForwardAxisValue == 0.f && LastMoveRightAxisValue != 0.f)
	{
		return LastMoveRightAxisValue > 0.f ? EPlayerMoveDirection::Right : EPlayerMoveDirection::Left;
	}

	if (LastMoveForwardAxisValue != 0.f && LastMoveRightAxisValue != 0.f)
	{
		// Right Forward
		if (LastMoveForwardAxisValue > 0.f && LastMoveRightAxisValue > 0.f)
		{
			return EPlayerMoveDirection::RightForward;
		}
		// Left Forward
		if (LastMoveForwardAxisValue > 0.f && LastMoveRightAxisValue < 0.f)
		{
			return EPlayerMoveDirection::LeftForward;
		}
		// Right Backward
		if (LastMoveForwardAxisValue < 0.f && LastMoveRightAxisValue > 0.f)
		{
			return EPlayerMoveDirection::RightBackward;
		}
		// Left Backward
		if (LastMoveForwardAxisValue < 0.f && LastMoveRightAxisValue < 0.f)
		{
			return EPlayerMoveDirection::LeftBackward;
		}
	}
	
	return EPlayerMoveDirection::Forward;	
}

void UZSAnimInstance::SetCharacterFood(ECharacterFootType NewValue)
{
	CharacterFoot = NewValue;
}

