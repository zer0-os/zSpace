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

	// PRINT_TIME(FString::SanitizeFloat(Direction), 0.f);
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

