// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "ZSAnimationTypes.h"
#include "Animation/AnimInstance.h"
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
	class UZSCharacterMovementComponent* CharacterMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Direction = 0.f;	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 bIsInAir : 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 bIsCrouching : 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EPlayerGait PlayerGait;

protected:
	UFUNCTION()
	void OnChangedPlayerGait(EPlayerGait NewValue);

	UFUNCTION(BlueprintPure)
	bool CurveIsActive(const EAnimCurveType& AnimCurveType, const FName& CurveName) const;
};
