// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimInstance.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"
#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/zSpace.h"

void UZSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	check(StartMovementAnimMontage);
	check(StopMovementAnimMontage);

	CharacterRef = Cast<AZSCharacterWithAbilities>(GetOwningActor());
	if (IsValid(CharacterRef))
	{
		CharacterMovementComponent = Cast<UZSCharacterMovementComponent>(CharacterRef->GetCharacterMovement());
		if (IsValid(CharacterMovementComponent))
		{
			CharacterMovementComponent->OnChangedPlayerGait.AddUniqueDynamic(this, &UZSAnimInstance::OnChangedPlayerGait);
		}
		CharacterRef->OnChangeAnimationState.AddUniqueDynamic(this, &UZSAnimInstance::OnChangedAnimationState);
	}

	OnMontageBlendingOut.AddUniqueDynamic(this, &UZSAnimInstance::EventOnMontageBlendingOut);
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
	// LastMoveForwardAxisValue = CharacterRef->GetLastMoveForwardAxisValue();
	MoveRightAxisValue = CharacterRef->GetMoveRightAxisValue();
	// LastMoveRightAxisValue = CharacterRef->GetLastMoveRightAxisValue();

	PlayerMoveDirection = CalculatePlayerMoveDirection();
	
	CharacterRelativeRotation = CharacterRef->GetCharacterRelativeRotation();

	StartPlayerMoveDirection = CalculateStartMoveDirection();
	
	MoveInputKeyTimeDownAverage = CharacterRef->GetMoveInputKeyTimeDownAverage();

	bIsDeath = CharacterRef->GetIsDeath();
	if (bIsDeath)
	{
		if (!IsValid(DeathAnimation))
		{
			const int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, DeathAnimations.Num() - 1);
			DeathAnimation = DeathAnimations[RandomIndex];
		}
	}
}

void UZSAnimInstance::OnChangedPlayerGait(EPlayerGait NewValue)
{
	PlayerGait = NewValue;
}

void UZSAnimInstance::OnChangedAnimationState(const EAnimationState CurrentValue)
{
	AnimationState = CurrentValue;	
}

bool UZSAnimInstance::CurveIsActive(const EAnimCurveType& AnimCurveType, const FName& CurveName) const
{
	TArray<FName> OutNames;
	GetActiveCurveNames(AnimCurveType, OutNames);
	const int32 Index = OutNames.Find(CurveName);

	return OutNames.IsValidIndex(Index);
}

void UZSAnimInstance::EventOnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (bIsMoveInputPressed)
		{
			if (StartMovementAnimMontage->HasThisMontage(Montage))
			{
				CharacterRef->Server_SetAnimationState(EAnimationState::LoopingInPlaceAnimation);
			}
			if (StopMovementAnimMontage->HasThisMontage(Montage))
			{
				CharacterRef->Server_SetAnimationState(EAnimationState::StopMovingAnimation);
			}
			if (Montage == CharacterRef->GetAttackMontage())
			{
				if (bIsMoveInputPressed)
				{
					CharacterRef->Server_SetAnimationState(EAnimationState::LoopingInPlaceAnimation);
				}
				else
				{
					CharacterRef->Server_SetAnimationState(EAnimationState::Standing);
				}
			}
		}
	}
}

EPlayerMoveDirection UZSAnimInstance::GetMoveDirection() const
{
	float Yaw = UKismetMathLibrary::NormalizeAxis(CharacterRelativeRotation);
	Yaw = UKismetMathLibrary::ClampAxis(Yaw);

	const EPlayerMoveDirection& PredictPlayerMoveDirection = PlayerMoveDirection;
	const float Angle = GetPlayerMoveDirectionAsAngle(PredictPlayerMoveDirection);
	
	Yaw = UKismetMathLibrary::ClampAxis(Yaw + Angle);
	
	auto NearlyEqual = [this, Yaw](const float& Value) -> bool
	{
		return UKismetMathLibrary::NearlyEqual_FloatFloat(Yaw, Value, 22.5);
	};

	const bool bIsForward = NearlyEqual(360.f) || NearlyEqual(0.f);
	
	// Forward
	if (bIsForward) return EPlayerMoveDirection::Forward;

	// Right Forward
	if (NearlyEqual(45.f)) return EPlayerMoveDirection::RightForward;
	
	// Right
	if (NearlyEqual(90.f)) return EPlayerMoveDirection::Right;

	// Backward Right
	if (NearlyEqual(135.f)) return EPlayerMoveDirection::RightBackward;

	// Backward
	if (NearlyEqual(180.f)) return EPlayerMoveDirection::Backward;
	
	// Left Backward
	if (NearlyEqual(225.f)) return EPlayerMoveDirection::LeftBackward;
	
	// Left 
	if (NearlyEqual(270.f)) return EPlayerMoveDirection::Left;

	// Left Forward
	if (NearlyEqual(315.f)) return EPlayerMoveDirection::LeftForward;

	return EPlayerMoveDirection::None;
}

EPlayerMoveDirection UZSAnimInstance::ReversMoveDirection(const EPlayerMoveDirection& Value)
{
	if (Value == EPlayerMoveDirection::Right)
	{
		return EPlayerMoveDirection::Left;
	}
	if (Value == EPlayerMoveDirection::Left)
	{
		return EPlayerMoveDirection::Right;
	}
	if (Value == EPlayerMoveDirection::LeftBackward)
	{
		return EPlayerMoveDirection::RightBackward;
	}
	if (Value == EPlayerMoveDirection::RightBackward)
	{
		return EPlayerMoveDirection::LeftBackward;
	}
	
	return Value;
}

EPlayerMoveDirection UZSAnimInstance::CalculatePlayerMoveDirection() const
{
	if (MoveForwardAxisValue == 0.f && MoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::None;
	}
	
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
	
	return EPlayerMoveDirection::None;
}

EPlayerMoveDirection UZSAnimInstance::LastCalculatePlayerMoveDirection() const
{
	if (LastMoveForwardAxisValue == 0.f && LastMoveRightAxisValue == 0.f)
	{
		return EPlayerMoveDirection::None;
	}
	
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
	
	return EPlayerMoveDirection::None;
}

EPlayerMoveDirection UZSAnimInstance::CalculateStartMoveDirection() const
{
	const EPlayerMoveDirection& L_PlayerMoveDirection = GetMoveDirection();
	
	return L_PlayerMoveDirection;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
float UZSAnimInstance::GetPlayerMoveDirectionAsAngle(const EPlayerMoveDirection& P_PlayerMoveDirection) const
{
	switch (P_PlayerMoveDirection)
	{
	case EPlayerMoveDirection::None: break;
	case EPlayerMoveDirection::Forward: return 0.f;;
	case EPlayerMoveDirection::RightForward: return 45.f;;
	case EPlayerMoveDirection::LeftForward: return -45.f;
	case EPlayerMoveDirection::Backward: return 180.f;
	case EPlayerMoveDirection::RightBackward: return 135.f;
	case EPlayerMoveDirection::LeftBackward: return -135.f;
	case EPlayerMoveDirection::Right: return 90.f;
	case EPlayerMoveDirection::Left: return -90.f;
	default: ;
	}

	return 0.f;
}

void UZSAnimInstance::SetCharacterFood(ECharacterFootType NewValue)
{
	CharacterFoot = NewValue;
}

