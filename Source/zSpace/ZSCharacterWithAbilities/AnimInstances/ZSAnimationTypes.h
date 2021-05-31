// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

#include "ZSAnimationTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerGait : uint8
{
	Standing = 0,
	Walking = 1,
	Running = 2,
	Sprinting = 3,

	Crouching = 4,
};

UENUM(BlueprintType)
enum class EPlayerMoveDirection : uint8
{
	None,
	
	Forward,
	RightForward,
	LeftForward,
	Backward,
	RightBackward,
	LeftBackward,
	Right,
	Left,
};

USTRUCT(BlueprintType)
struct FAnimMontageLocomotion
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* AnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPlayerGait PlayerGait;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterFootType CharacterFoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPlayerMoveDirection PlayerMoveDirection;
};

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	Standing,

	// Movement
	StartMovingAnimation,
	LoopingInPlaceAnimation,
	StopMovingAnimation,

	// Attack
	AttackingAnimation,
};

UCLASS(BlueprintType)
class UAnimMontageLocomotionDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAnimMontageLocomotion> AnimMontageLocomotionArray;

	UFUNCTION(BlueprintPure)
	class UAnimMontage* GetAnimMontageByGaitAndFoot(const EPlayerGait& PlayerGait, const ECharacterFootType& CharacterFoot);
	
	UFUNCTION(BlueprintPure)
	class UAnimMontage* GetAnimMontageByGaitAndDirection(const EPlayerGait& PlayerGait, const EPlayerMoveDirection PlayerMoveDirection);

	UFUNCTION(BlueprintPure)
	bool HasThisMontage(class UAnimMontage* ThisMontage);
	
	UFUNCTION(BlueprintPure)
	TArray<class UAnimMontage*> GetAllMontages() const;
	
};