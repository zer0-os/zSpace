// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "ZSAnimationTypes.h"
#include "Animation/AnimInstance.h"
#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 bIsMoveInputPressed : 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MoveForwardAxisValue = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float LastMoveForwardAxisValue = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MoveRightAxisValue = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float LastMoveRightAxisValue = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 bIsMovingOnlyForward : 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECharacterFootType CharacterFoot = ECharacterFootType::NONE;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EPlayerMoveDirection PlayerMoveDirection;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EPlayerMoveDirection LastPlayerMoveDirection;
	
protected:
	UFUNCTION()
	void OnChangedPlayerGait(EPlayerGait NewValue);

	UFUNCTION(BlueprintPure)
	bool CurveIsActive(const EAnimCurveType& AnimCurveType, const FName& CurveName) const;
	
	UFUNCTION(BlueprintPure)
	bool IsMovingOnlyForward() const;
	
	UFUNCTION(BlueprintPure)
	bool IsMovedOnlyForward() const;

public:
	UFUNCTION(BlueprintPure)
	EPlayerMoveDirection CalculatePlayerMoveDirection() const;
	
	UFUNCTION(BlueprintPure)
	EPlayerMoveDirection LastCalculatePlayerMoveDirection() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCharacterFood(ECharacterFootType NewValue);
	
};
