// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
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

	PlayerMoveDirection = CalculatePlayerMoveDirection();
	LastPlayerMoveDirection = LastCalculatePlayerMoveDirection();
	
	CharacterRelativeRotation = CharacterRef->GetCharacterRelativeRotation();
	StartPlayerMoveDirection = CalculateStartMoveDirection();

	MoveInputKeyTimeDownAverage = CharacterRef->GetMoveInputKeyTimeDownAverage();
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

EPlayerMoveDirection UZSAnimInstance::GetMoveDirection() const
{
	auto ValueIsInRange = [this](const int32 Value, const int32 Min, const int32 Max) -> bool
	{
		return Value > Min && Value < Max;
	};

	float Yaw = UKismetMathLibrary::NormalizeAxis(CharacterRelativeRotation);
	Yaw = UKismetMathLibrary::ClampAxis(Yaw);
	
	const bool bIsForward = ValueIsInRange(Yaw, 337.5f, 360.f) || ValueIsInRange(Yaw, -1.f, 22.5);
	
	// Forward
	if (bIsForward) return EPlayerMoveDirection::Forward;

	// Right Forward
	if (ValueIsInRange(Yaw, 22.5, 67.5f)) return EPlayerMoveDirection::RightForward;
	
	// Right
	if (ValueIsInRange(Yaw, 67.5f, 112.5f)) return EPlayerMoveDirection::Right;

	// Backward Right
	if (ValueIsInRange(Yaw, 112.5f, 157.5f)) return EPlayerMoveDirection::RightBackward;

	// Backward
	if (ValueIsInRange(Yaw, 157.5f, 202.5f)) return EPlayerMoveDirection::Backward;
	
	// Left Backward
	if (ValueIsInRange(Yaw, 202.5f, 247.5f)) return EPlayerMoveDirection::LeftBackward;
	
	// Left 
	if (ValueIsInRange(Yaw, 247.5f, 292.5f)) return EPlayerMoveDirection::Left;

	// Left Forward
	if (ValueIsInRange(Yaw, 292.5f, 337.5f)) return EPlayerMoveDirection::LeftForward;

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
	EPlayerMoveDirection L_PlayerMoveDirection = GetMoveDirection();
	
	float Yaw = UKismetMathLibrary::NormalizeAxis(CharacterRelativeRotation);
	Yaw = UKismetMathLibrary::ClampAxis(Yaw);
	
	const auto IsEqual = UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::NormalizeAxis(Yaw), 0.f, 85.f);
	if (!IsEqual)
	{
		L_PlayerMoveDirection = ReversMoveDirection(L_PlayerMoveDirection);
		const EPlayerMoveDirection& PredictPlayerMoveDirection = CalculatePlayerMoveDirection();
		
		if (PredictPlayerMoveDirection == L_PlayerMoveDirection)
		{
			L_PlayerMoveDirection = EPlayerMoveDirection::Forward;
		}
		if (L_PlayerMoveDirection == EPlayerMoveDirection::Right && PredictPlayerMoveDirection == EPlayerMoveDirection::Left)
		{
			L_PlayerMoveDirection = EPlayerMoveDirection::Backward;
		}
		if (L_PlayerMoveDirection == EPlayerMoveDirection::Left && PredictPlayerMoveDirection == EPlayerMoveDirection::Right)
		{
			L_PlayerMoveDirection = EPlayerMoveDirection::Backward;
		}
	}
	
	// UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerMoveDirection"), true);
	// if (IsValid(EnumPtr))
	// {
	// 	const FName& EnumName = EnumPtr->GetNameByIndex(static_cast<uint8>(L_PlayerMoveDirection));
	// 	const FString EnumStr = EnumName.ToString().Replace(TEXT("EPlayerMoveDirection::"), TEXT(""));
	// 	PRINT_TIME(EnumStr, 0.f);
	// }
	
	return L_PlayerMoveDirection;
}

float UZSAnimInstance::GetPlayerMoveDirectionAsAngle() const
{
	const EPlayerMoveDirection& L_PlayerMoveDirection = CalculatePlayerMoveDirection();

	switch (L_PlayerMoveDirection)
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

