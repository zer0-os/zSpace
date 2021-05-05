// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"

#include "ZSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual  void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	class AZSCharacterWithAbilities* CharacterRef = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float Direction = 0.f;	

	UPROPERTY(BlueprintReadOnly)
	uint8 bIsInAir : 1;
	
};
