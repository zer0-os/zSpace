// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacterMovementComponent.h"
#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimationTypes.h"

#include "ZSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSCharacterMovementComponent : public UOWSCharacterMovementComponent
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementModeChanged, EMovementMode, PreviousMovementMode, EMovementMode, CurrentMovementMode,  uint8, PreviousCustomMode);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedPlayerGait, EPlayerGait, CurrentPlayerGait);

public:
	UZSCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer);

	virtual void OnMovementModeChanged ( EMovementMode NewPreviousMovementMode, uint8 NewPreviousCustomMode ) override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetMaxSpeed() const override;

protected:
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

public:
	UPROPERTY(BlueprintAssignable)
	FMovementModeChanged OnMovementModeChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnChangedPlayerGait OnChangedPlayerGait;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_ChangeMovementMode(EPlayerGait NewValue);

	UFUNCTION(BlueprintPure)
	FORCEINLINE EPlayerGait GetPlayerGait() const { return PlayerGait; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EPlayerGait GetPlayerGaitPreStanding() const { return PlayerGaitPreStanding; }
	
	// For Tick Update
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool NeedReplicatePlayerGait(const EPlayerGait& CheckPlayerGait) const { return PlayerGait != CheckPlayerGait; }

	// For Tick Update
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool NeedReplicateMaxWalkSpeed(const float& CheckValue) const { return CheckValue != MaxWalkSpeed; }
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	EPlayerGait PlayerGait;

	UPROPERTY(Replicated, BlueprintReadOnly)
	EPlayerGait PlayerGaitPreStanding = EPlayerGait::Standing;
	
	UPROPERTY(BlueprintReadOnly)
	class UAnimInstance* AnimInstance = nullptr;

	UPROPERTY()
	class AZSCharacterWithAbilities* OwnerZSCharacter = nullptr;

protected:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_OnChangedPlayerGait(EPlayerGait NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetMaxWalkSpeed(const float& NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetOrientRotationToMovement(bool NewValue);
	
	class UAnimInstance* GetAnimInstance() const;

	void UpdateMaxWalkSpeed();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	
	virtual bool IsWalkable(const FHitResult& Hit) const override;
};
