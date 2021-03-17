// Copyright 2018 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSPawnWithAbilities.h"
#include "GameFramework/PawnMovementComponent.h"
#include "OWSPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class OWSPLUGIN_API UOWSPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

public:

	float GetSimulationTimeStep(float RemainingTime, int32 Iterations) const;

	/**
	 * Max time delta for each discrete simulation step.
	 * Used primarily in the the more advanced movement modes that break up larger time steps (usually those applying gravity such as falling and walking).
	 * Lowering this value can address issues with fast-moving objects or complex collision scenarios, at the cost of performance.
	 *
	 * WARNING: if (MaxSimulationTimeStep * MaxSimulationIterations) is too low for the min framerate, the last simulation step may exceed MaxSimulationTimeStep to complete the simulation.
	 * @see MaxSimulationIterations
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "0.0166", ClampMax = "0.50", UIMin = "0.0166", UIMax = "0.50"))
		float MaxSimulationTimeStep;

	/**
	 * Max number of iterations used for each discrete simulation step.
	 * Used primarily in the the more advanced movement modes that break up larger time steps (usually those applying gravity such as falling and walking).
	 * Increasing this value can address issues with fast-moving objects or complex collision scenarios, at the cost of performance.
	 *
	 * WARNING: if (MaxSimulationTimeStep * MaxSimulationIterations) is too low for the min framerate, the last simulation step may exceed MaxSimulationTimeStep to complete the simulation.
	 * @see MaxSimulationTimeStep
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "1", ClampMax = "25", UIMin = "1", UIMax = "25"))
		int32 MaxSimulationIterations;

	/**
	 * Actor's current movement mode (walking, falling, etc).
	 *    - walking:  Walking on a surface, under the effects of friction, and able to "step up" barriers. Vertical velocity is zero.
	 *    - falling:  Falling under the effects of gravity, after jumping or walking off the edge of a surface.
	 *    - flying:   Flying, ignoring the effects of gravity.
	 * This is automatically replicated through the Character owner and for client-server movement functions.
	 */
	UPROPERTY(Category = "Character Movement: MovementMode", BlueprintReadOnly)
		TEnumAsByte<enum EMovementMode> MovementMode;

	/** Custom gravity scale. Gravity is multiplied by this amount for the character. */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
		float GravityScale;

	/** Maximum height character can step up */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxStepHeight;

	/** Initial velocity (instantaneous vertical acceleration) when jumping. */
	UPROPERTY(Category = "Character Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Jump Z Velocity", ClampMin = "0", UIMin = "0"))
		float JumpZVelocity;

	/** Fraction of JumpZVelocity to use when automatically "jumping off" of a base actor that's not allowed to be a base for a character. (For example, if you're not allowed to stand on other players.) */
	UPROPERTY(Category = "Character Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "0", UIMin = "0"))
		float JumpOffJumpZFactor;

	/** The maximum ground speed when walking. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxWalkSpeed;

	/** The maximum ground speed when walking and crouched. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxWalkSpeedCrouched;

	/** The maximum flying speed. */
	UPROPERTY(Category = "Character Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxFlySpeed;

	/** Max Acceleration (rate of change of velocity) */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxAcceleration;

	float GetGravityZ() const;

	float GetMaxSpeed() const;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	/** Return true if we have a valid OWSPawnWithAbilitiesOwner and UpdatedComponent. */
	virtual bool HasValidData() const;

	/** Get the Character that owns UpdatedComponent. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
		AOWSPawnWithAbilities* GetOWSPawnWithAbilitiesOwner() const;

	//void AdjustProxyCapsuleSize();

	/** Information about the floor the Character is standing on (updated only during walking movement). */
	UPROPERTY(Category = "Character Movement: Walking", VisibleInstanceOnly, BlueprintReadOnly)
		FFindFloorResult CurrentFloor;

	/**
	 * Signals that smoothed position/rotation has reached target, and no more smoothing is necessary until a future update.
	 * This is used as an optimization to skip calls to SmoothClientPosition() when true. SmoothCorrection() sets it false when a new network update is received.
	 * SmoothClientPosition_Interpolate() sets this to true when the interpolation reaches the target, before one last call to SmoothClientPosition_UpdateVisuals().
	 * If this is not desired, override SmoothClientPosition() to always set this to false to avoid this feature.
	 */
	uint8 bNetworkSmoothingComplete : 1;

	/**
	 * If true, high-level movement updates will be wrapped in a movement scope that accumulates updates and defers a bulk of the work until the end.
	 * When enabled, touch and hit events will not be triggered until the end of multiple moves within an update, which can improve performance.
	 *
	 * @see FScopedMovementUpdate
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, AdvancedDisplay)
		uint8 bEnableScopedMovementUpdates : 1;

	/**
	 * Force the Character in MOVE_Walking to do a check for a valid floor even if he hasn't moved. Cleared after next floor check.
	 * Normally if bAlwaysCheckFloor is false we try to avoid the floor check unless some conditions are met, but this can be used to force the next check to always run.
	 */
	UPROPERTY(Category = "Character Movement: Walking", VisibleInstanceOnly, BlueprintReadWrite, AdvancedDisplay)
		uint8 bForceNextFloorCheck : 1;

	/** Used by movement code to determine if a change in position is based on normal movement or a teleport. If not a teleport, velocity can be recomputed based on the change in position. */
	UPROPERTY(Category = "Character Movement (General Settings)", Transient, VisibleInstanceOnly, BlueprintReadWrite)
		uint8 bJustTeleported : 1;

	/** True when a network replication update is received for simulated proxies. */
	UPROPERTY(Transient)
		uint8 bNetworkUpdateReceived : 1;

	/** True when the networked movement mode has been replicated. */
	UPROPERTY(Transient)
		uint8 bNetworkMovementModeChanged : 1;

	/**
	 * Whether we always force floor checks for stationary Characters while walking.
	 * Normally floor checks are avoided if possible when not moving, but this can be used to force them if there are use-cases where they are being skipped erroneously
	 * (such as objects moving up into the character from below).
	 */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
		uint8 bAlwaysCheckFloor : 1;

	void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode);

	/** Special Tick for Simulated Proxies */
	void SimulatedTick(float DeltaSeconds);

	void SimulateMovement(float DeltaSeconds);

	void SmoothClientPosition(float DeltaSeconds);	

	void SmoothClientPosition_UpdateVisuals();

	void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const;

	/** Minimum delta time considered when ticking. Delta times below this are not considered. This is a very small non-zero value to avoid potential divide-by-zero in simulation code. */
	static const float MIN_TICK_TIME;

	/** Minimum acceptable distance for Character capsule to float above floor when walking. */
	static const float MIN_FLOOR_DIST;

	/** Maximum acceptable distance for Character capsule to float above floor when walking. */
	static const float MAX_FLOOR_DIST;

protected:

	/**
 * Current acceleration vector (with magnitude).
 * This is calculated each update based on the input vector and the constraints of MaxAcceleration and the current movement mode.
 */
	UPROPERTY()
		FVector Acceleration;

	/**
	 * Rotation after last PerformMovement or SimulateMovement update.
	 */
	UPROPERTY()
		FQuat LastUpdateRotation;

	/**
	 * Location after last PerformMovement or SimulateMovement update. Used internally to detect changes in position from outside character movement to try to validate the current floor.
	 */
	UPROPERTY()
		FVector LastUpdateLocation;

	/**
	 * Velocity after last PerformMovement or SimulateMovement update. Used internally to detect changes in velocity from external sources.
	 */
	UPROPERTY()
		FVector LastUpdateVelocity;

	/** OWSPawnWithAbilitiesOwner movement component belongs to */
	UPROPERTY(Transient, DuplicateTransient)
		AOWSPawnWithAbilities* OWSPawnWithAbilitiesOwner;


private:

	FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const;
	void PhysFalling(float deltaTime, int32 Iterations);

};

FORCEINLINE AOWSPawnWithAbilities* UOWSPawnMovementComponent::GetOWSPawnWithAbilitiesOwner() const
{
	return OWSPawnWithAbilitiesOwner;
}