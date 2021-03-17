// Copyright 2018 Sabre Dart Studios


#include "OWSPawnMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"

const float UOWSPawnMovementComponent::MIN_TICK_TIME = 1e-6f;
const float UOWSPawnMovementComponent::MIN_FLOOR_DIST = 1.9f;
const float UOWSPawnMovementComponent::MAX_FLOOR_DIST = 2.4f;

/*
void FFindFloorResult::SetFromSweep(const FHitResult& InHit, const float InSweepFloorDist, const bool bIsWalkableFloor)
{
	bBlockingHit = InHit.IsValidBlockingHit();
	bWalkableFloor = bIsWalkableFloor;
	bLineTrace = false;
	FloorDist = InSweepFloorDist;
	LineDist = 0.f;
	HitResult = InHit;
}

void FFindFloorResult::SetFromLineTrace(const FHitResult& InHit, const float InSweepFloorDist, const float InLineDist, const bool bIsWalkableFloor)
{
	// We require a sweep that hit if we are going to use a line result.
	check(HitResult.bBlockingHit);
	if (HitResult.bBlockingHit && InHit.bBlockingHit)
	{
		// Override most of the sweep result with the line result, but save some values
		FHitResult OldHit(HitResult);
		HitResult = InHit;

		// Restore some of the old values. We want the new normals and hit actor, however.
		HitResult.Time = OldHit.Time;
		HitResult.ImpactPoint = OldHit.ImpactPoint;
		HitResult.Location = OldHit.Location;
		HitResult.TraceStart = OldHit.TraceStart;
		HitResult.TraceEnd = OldHit.TraceEnd;

		bLineTrace = true;
		FloorDist = InSweepFloorDist;
		LineDist = InLineDist;
		bWalkableFloor = bIsWalkableFloor;
	}
}
*/


UOWSPawnMovementComponent::UOWSPawnMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*RandomStream.Initialize(FApp::bUseFixedSeed ? GetFName() : NAME_None);

	PostPhysicsTickFunction.bCanEverTick = true;
	PostPhysicsTickFunction.bStartWithTickEnabled = false;
	PostPhysicsTickFunction.SetTickFunctionEnable(false);
	PostPhysicsTickFunction.TickGroup = TG_PostPhysics;

	bApplyGravityWhileJumping = true;
	*/

	GravityScale = 1.f;
	//GroundFriction = 8.0f;
	JumpZVelocity = 420.0f;
	JumpOffJumpZFactor = 0.5f;
	//RotationRate = FRotator(0.f, 360.0f, 0.0f);
	//SetWalkableFloorZ(0.71f);

	MaxStepHeight = 45.0f;
	//PerchRadiusThreshold = 0.0f;
	//PerchAdditionalHeight = 40.f;

	MaxFlySpeed = 600.0f;
	MaxWalkSpeed = 600.0f;
	//MaxSwimSpeed = 300.0f;
	//MaxCustomMovementSpeed = MaxWalkSpeed;

	MaxSimulationTimeStep = 0.05f;
	MaxSimulationIterations = 8;
	//MaxJumpApexAttemptsPerSimulation = 2;
	//NumJumpApexAttempts = 0;

	/*
	MaxDepenetrationWithGeometry = 500.f;
	MaxDepenetrationWithGeometryAsProxy = 100.f;
	MaxDepenetrationWithPawn = 100.f;
	MaxDepenetrationWithPawnAsProxy = 2.f;

	// Set to match EVectorQuantization::RoundTwoDecimals
	NetProxyShrinkRadius = 0.01f;
	NetProxyShrinkHalfHeight = 0.01f;
	
	NetworkSimulatedSmoothLocationTime = 0.100f;
	NetworkSimulatedSmoothRotationTime = 0.050f;
	ListenServerNetworkSimulatedSmoothLocationTime = 0.040f;
	ListenServerNetworkSimulatedSmoothRotationTime = 0.033f;
	NetworkMaxSmoothUpdateDistance = 256.f;
	NetworkNoSmoothUpdateDistance = 384.f;
	NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	ServerLastClientGoodMoveAckTime = -1.f;
	ServerLastClientAdjustmentTime = -1.f;
	NetworkMinTimeBetweenClientAckGoodMoves = 0.10f;
	NetworkMinTimeBetweenClientAdjustments = 0.10f;
	NetworkMinTimeBetweenClientAdjustmentsLargeCorrection = 0.05f;
	NetworkLargeClientCorrectionDistance = 15.0f;
	*/

	MaxWalkSpeedCrouched = MaxWalkSpeed * 0.5f;
	/*
	MaxOutOfWaterStepHeight = 40.0f;
	OutofWaterZ = 420.0f;
	AirControl = 0.05f;
	AirControlBoostMultiplier = 2.f;
	AirControlBoostVelocityThreshold = 25.f;
	FallingLateralFriction = 0.f;
	*/
	MaxAcceleration = 2048.0f;
	/*
	BrakingFrictionFactor = 2.0f; // Historical value, 1 would be more appropriate.
	BrakingSubStepTime = 1.0f / 33.0f;
	BrakingDecelerationWalking = MaxAcceleration;
	BrakingDecelerationFalling = 0.f;
	BrakingDecelerationFlying = 0.f;
	BrakingDecelerationSwimming = 0.f;
	LedgeCheckThreshold = 4.0f;
	JumpOutOfWaterPitch = 11.25f;
	*/

#if WITH_EDITORONLY_DATA
	/*
	CrouchedSpeedMultiplier_DEPRECATED = 0.5f;
	UpperImpactNormalScale_DEPRECATED = 0.5f;
	bForceBraking_DEPRECATED = false;
	*/
#endif

	//Mass = 100.0f;
	bJustTeleported = true;
	//CrouchedHalfHeight = 40.0f;
	//Buoyancy = 1.0f;
	LastUpdateRotation = FQuat::Identity;
	LastUpdateVelocity = FVector::ZeroVector;
	/*
	PendingImpulseToApply = FVector::ZeroVector;
	PendingLaunchVelocity = FVector::ZeroVector;
	DefaultWaterMovementMode = MOVE_Swimming;
	DefaultLandMovementMode = MOVE_Walking;
	GroundMovementMode = MOVE_Walking;
	*/
	bForceNextFloorCheck = true;
	/*
	bShrinkProxyCapsule = true;
	bCanWalkOffLedges = true;
	bCanWalkOffLedgesWhenCrouching = false;
	*/
	bNetworkSmoothingComplete = true; // Initially true until we get a net update, so we don't try to smooth to an uninitialized value.
	/*
	bWantsToLeaveNavWalking = false;
	bIsNavWalkingOnServer = false;
	bSweepWhileNavWalking = true;
	bNeedsSweepWhileWalkingUpdate = false;
	
	bEnablePhysicsInteraction = true;
	StandingDownwardForceScale = 1.0f;
	InitialPushForceFactor = 500.0f;
	PushForceFactor = 750000.0f;
	PushForcePointZOffsetFactor = -0.75f;
	bPushForceUsingZOffset = false;
	bPushForceScaledToMass = false;
	bScalePushForceToVelocity = true;

	TouchForceFactor = 1.0f;
	bTouchForceScaledToMass = true;
	MinTouchForce = -1.0f;
	MaxTouchForce = 250.0f;
	RepulsionForce = 2.5f;

	bAllowPhysicsRotationDuringAnimRootMotion = false; // Old default behavior.
	bUseControllerDesiredRotation = false;

	bUseSeparateBrakingFriction = false; // Old default behavior.

	bMaintainHorizontalGroundVelocity = true;
	bImpartBaseVelocityX = true;
	
	bImpartBaseVelocityY = true;
	bImpartBaseVelocityZ = true;
	bImpartBaseAngularVelocity = true;
	bIgnoreClientMovementErrorChecksAndCorrection = false;
	bServerAcceptClientAuthoritativePosition = false;
	*/

	bAlwaysCheckFloor = true;

	// default character can jump, walk, and swim
	NavAgentProps.bCanJump = true;
	NavAgentProps.bCanWalk = true;
	NavAgentProps.bCanSwim = true;
	ResetMoveState();

	/*
	ClientPredictionData = NULL;
	ServerPredictionData = NULL;

	// This should be greater than tolerated player timeout * 2.
	MinTimeBetweenTimeStampResets = 4.f * 60.f;
	LastTimeStampResetServerTime = 0.f;
	*/

	bEnableScopedMovementUpdates = true;
	// Disabled by default since it can be a subtle behavior change, you should opt in if you want to accept that.
	/*
	bEnableServerDualMoveScopedMovementUpdates = false;

	bRequestedMoveUseAcceleration = true;
	bUseRVOAvoidance = false;
	bUseRVOPostProcess = false;
	AvoidanceLockVelocity = FVector::ZeroVector;
	AvoidanceLockTimer = 0.0f;
	AvoidanceGroup.bGroup0 = true;
	GroupsToAvoid.Packed = 0xFFFFFFFF;
	GroupsToIgnore.Packed = 0;
	AvoidanceConsiderationRadius = 500.0f;

	OldBaseQuat = FQuat::Identity;
	OldBaseLocation = FVector::ZeroVector;

	NavMeshProjectionInterval = 0.1f;
	NavMeshProjectionInterpSpeed = 12.f;
	NavMeshProjectionHeightScaleUp = 0.67f;
	NavMeshProjectionHeightScaleDown = 1.0f;
	NavWalkingFloorDistTolerance = 10.0f;
	*/
}

void UOWSPawnMovementComponent::PostLoad()
{
	Super::PostLoad();

	OWSPawnWithAbilitiesOwner = Cast<AOWSPawnWithAbilities>(PawnOwner);
}

float UOWSPawnMovementComponent::GetGravityZ() const
{
	return Super::GetGravityZ() * GravityScale;
}

float UOWSPawnMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed;
	case MOVE_Falling:
		return MaxWalkSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

FVector UOWSPawnMovementComponent::NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const
{
	FVector Result = InitialVelocity;

	if (DeltaTime > 0.f)
	{
		// Apply gravity.
		Result += Gravity * DeltaTime;

		// Don't exceed terminal velocity.
		const float TerminalLimit = FMath::Abs(GetPhysicsVolume()->TerminalVelocity);
		if (Result.SizeSquared() > FMath::Square(TerminalLimit))
		{
			const FVector GravityDir = Gravity.GetSafeNormal();
			if ((Result | GravityDir) > TerminalLimit)
			{
				Result = FVector::PointPlaneProject(Result, FVector::ZeroVector, GravityDir) + GravityDir * TerminalLimit;
			}
		}
	}

	return Result;
}


void UOWSPawnMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	float remainingTime = deltaTime;

	Iterations++;
	float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
	remainingTime -= timeTick;

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FQuat PawnRotation = UpdatedComponent->GetComponentQuat();

	const FVector OldVelocity = Velocity;

	// Compute current gravity
	const FVector Gravity(0.f, 0.f, GetGravityZ());
	float GravityTime = timeTick;

	// Apply gravity
	Velocity = NewFallVelocity(Velocity, Gravity, GravityTime);

	// Compute change in position (using midpoint integration method).
	FVector Adjusted = 0.5f*(OldVelocity + Velocity) * timeTick;

	// Move
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, PawnRotation, true, Hit);

}

float UOWSPawnMovementComponent::GetSimulationTimeStep(float RemainingTime, int32 Iterations) const
{
	static uint32 s_WarningCount = 0;
	if (RemainingTime > MaxSimulationTimeStep)
	{
		if (Iterations < MaxSimulationIterations)
		{
			// Subdivide moves to be no longer than MaxSimulationTimeStep seconds
			RemainingTime = FMath::Min(MaxSimulationTimeStep, RemainingTime * 0.5f);
		}
		else
		{
			// If this is the last iteration, just use all the remaining time. This is usually better than cutting things short, as the simulation won't move far enough otherwise.
			// Print a throttled warning.
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if ((s_WarningCount++ < 100) || (GFrameCounter & 15) == 0)
			{
				//UE_LOG(OWS, Warning, TEXT("GetSimulationTimeStep() - Max iterations %d hit while remaining time %.6f > MaxSimulationTimeStep (%.3f) for '%s', movement '%s'"), MaxSimulationIterations, RemainingTime, MaxSimulationTimeStep, *GetNameSafe(CharacterOwner), *GetMovementName());
			}
#endif
		}
	}

	// no less than MIN_TICK_TIME (to avoid potential divide-by-zero during simulation).
	return FMath::Max(MIN_TICK_TIME, RemainingTime);
}


void UOWSPawnMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	if (NewUpdatedComponent)
	{
		const ACharacter* NewCharacterOwner = Cast<ACharacter>(NewUpdatedComponent->GetOwner());
		if (NewCharacterOwner == NULL)
		{
			UE_LOG(OWS, Error, TEXT("%s owned by %s must update a component owned by a Character"), *GetName(), *GetNameSafe(NewUpdatedComponent->GetOwner()));
			return;
		}

		// check that UpdatedComponent is a Capsule
		if (Cast<UCapsuleComponent>(NewUpdatedComponent) == NULL)
		{
			UE_LOG(OWS, Error, TEXT("%s owned by %s must update a capsule component"), *GetName(), *GetNameSafe(NewUpdatedComponent->GetOwner()));
			return;
		}
	}

	/*
	if (bMovementInProgress)
	{
		// failsafe to avoid crashes in CharacterMovement. 
		bDeferUpdateMoveComponent = true;
		DeferredUpdatedMoveComponent = NewUpdatedComponent;
		return;
	}
	bDeferUpdateMoveComponent = false;
	DeferredUpdatedMoveComponent = NULL;

	USceneComponent* OldUpdatedComponent = UpdatedComponent;
	UPrimitiveComponent* OldPrimitive = Cast<UPrimitiveComponent>(UpdatedComponent);
	if (IsValid(OldPrimitive) && OldPrimitive->OnComponentBeginOverlap.IsBound())
	{
		OldPrimitive->OnComponentBeginOverlap.RemoveDynamic(this, &UCharacterMovementComponent::CapsuleTouched);
	}
	*/

	Super::SetUpdatedComponent(NewUpdatedComponent);
	OWSPawnWithAbilitiesOwner = Cast<AOWSPawnWithAbilities>(PawnOwner);

	/*
	if (UpdatedComponent != OldUpdatedComponent)
	{
		ClearAccumulatedForces();
	}
	*/

	if (UpdatedComponent == NULL)
	{
		StopActiveMovement();
	}

	const bool bValidUpdatedPrimitive = IsValid(UpdatedPrimitive);

	/*
	if (bValidUpdatedPrimitive && bEnablePhysicsInteraction)
	{
		UpdatedPrimitive->OnComponentBeginOverlap.AddUniqueDynamic(this, &UCharacterMovementComponent::CapsuleTouched);
	}

	if (bNeedsSweepWhileWalkingUpdate)
	{
		bSweepWhileNavWalking = bValidUpdatedPrimitive ? UpdatedPrimitive->GetGenerateOverlapEvents() : false;
		bNeedsSweepWhileWalkingUpdate = false;
	}

	if (bUseRVOAvoidance && IsValid(NewUpdatedComponent))
	{
		UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();
		if (AvoidanceManager)
		{
			AvoidanceManager->RegisterMovementComponent(this, AvoidanceWeight);
		}
	}
	*/
}

bool UOWSPawnMovementComponent::HasValidData() const
{
	const bool bIsValid = UpdatedComponent && IsValid(OWSPawnWithAbilitiesOwner);

	return bIsValid;
}

void UOWSPawnMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	// If trying to use NavWalking but there is no navmesh, use walking instead.
	/*if (NewMovementMode == MOVE_NavWalking)
	{
		if (GetNavData() == nullptr)
		{
			NewMovementMode = MOVE_Walking;
		}
	}*/

	// Do nothing if nothing is changing.
	if (MovementMode == NewMovementMode)
	{
		return;
	}

	const EMovementMode PrevMovementMode = MovementMode;
	//const uint8 PrevCustomMode = CustomMovementMode;

	MovementMode = NewMovementMode;
	//CustomMovementMode = NewCustomMode;

	// We allow setting movement mode before we have a component to update, in case this happens at startup.
	if (!HasValidData())
	{
		return;
	}

	// Handle change in movement mode
	//OnMovementModeChanged(PrevMovementMode, PrevCustomMode);

	// @todo UE4 do we need to disable ragdoll physics here? Should this function do nothing if in ragdoll?
}

void UOWSPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	SetMovementMode(MOVE_Falling, 0);

	if (!HasValidData() || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Super tick may destroy/invalidate CharacterOwner or UpdatedComponent, so we need to re-check.
	if (!HasValidData())
	{
		return;
	}

	// See if we fell out of the world.
	const bool bIsSimulatingPhysics = UpdatedComponent->IsSimulatingPhysics();
	if (OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_Authority && (bIsSimulatingPhysics) && !OWSPawnWithAbilitiesOwner->CheckStillInWorld())
	{
		return;
	}

	// We don't update if simulating physics (eg ragdolls).
	if (bIsSimulatingPhysics)
	{
		// Update camera to ensure client gets updates even when physics move him far away from point where simulation started
		if (OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client))
		{
			MarkForClientCameraUpdate();
		}

		//ClearAccumulatedForces();
		return;
	}

	//Remove this!!!
	PhysFalling(DeltaTime, 1);
	//End Remove this!!!

	if (OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		/*if (bShrinkProxyCapsule)
		{
			AdjustProxyCapsuleSize();
		}*/
		SimulatedTick(DeltaTime);
	}
}

void UOWSPawnMovementComponent::SimulatedTick(float DeltaSeconds)
{
	//SCOPE_CYCLE_COUNTER(STAT_CharacterMovementSimulated);
	checkSlow(OWSPawnWithAbilitiesOwner != nullptr);

	if (OWSPawnWithAbilitiesOwner->IsReplicatingMovement() && UpdatedComponent)
	{
		USkeletalMeshComponent* Mesh = OWSPawnWithAbilitiesOwner->GetMesh();
		const FVector SavedMeshRelativeLocation = Mesh ? Mesh->GetRelativeLocation() : FVector::ZeroVector;
		const FQuat SavedCapsuleRotation = UpdatedComponent->GetComponentQuat();

		// Avoid moving the mesh during movement if SmoothClientPosition will take care of it.
		{
			SimulateMovement(DeltaSeconds);
		}
	}

	// Smooth mesh location after moving the capsule above.
	if (!bNetworkSmoothingComplete)
	{
		//SCOPE_CYCLE_COUNTER(STAT_CharacterMovementSmoothClientPosition);
		SmoothClientPosition(DeltaSeconds);
	}
	else
	{
		//UE_LOG(OWS, Verbose, TEXT("Skipping network smoothing for %s."), *GetNameSafe(OWSPawnWithAbilitiesOwner));
	}
}

void UOWSPawnMovementComponent::SimulateMovement(float DeltaSeconds)
{
	if (!HasValidData() || UpdatedComponent->Mobility != EComponentMobility::Movable || UpdatedComponent->IsSimulatingPhysics())
	{
		return;
	}

	const bool bIsSimulatedProxy = (OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_SimulatedProxy);

	const FRepMovement& ConstRepMovement = OWSPawnWithAbilitiesOwner->GetReplicatedMovement();

	// Workaround for replication not being updated initially
	if (bIsSimulatedProxy &&
		ConstRepMovement.Location.IsZero() &&
		ConstRepMovement.Rotation.IsZero() &&
		ConstRepMovement.LinearVelocity.IsZero())
	{
		return;
	}

/*
	// If base is not resolved on the client, we should not try to simulate at all
	if (OWSPawnWithAbilitiesOwner->GetReplicatedBasedMovement().IsBaseUnresolved())
	{
		UE_LOG(OWS, Verbose, TEXT("Base for simulated character '%s' is not resolved on client, skipping SimulateMovement"), *CharacterOwner->GetName());
		return;
	}

	FVector OldVelocity;
	FVector OldLocation;

	// Scoped updates can improve performance of multiple MoveComponent calls.
	{
		FScopedMovementUpdate ScopedMovementUpdate(UpdatedComponent, bEnableScopedMovementUpdates ? EScopedUpdate::DeferredUpdates : EScopedUpdate::ImmediateUpdates);

		bool bHandledNetUpdate = false;
		if (bIsSimulatedProxy)
		{
			// Handle network changes
			if (bNetworkUpdateReceived)
			{
				bNetworkUpdateReceived = false;
				bHandledNetUpdate = true;
				UE_LOG(OWS, Verbose, TEXT("Proxy %s received net update"), *OWSPawnWithAbilitiesOwner->GetName());
				if (bNetworkMovementModeChanged)
				{
					ApplyNetworkMovementMode(OWSPawnWithAbilitiesOwner->GetReplicatedMovementMode());
					bNetworkMovementModeChanged = false;
				}
				else if (bJustTeleported || bForceNextFloorCheck)
				{
					// Make sure floor is current. We will continue using the replicated base, if there was one.
					bJustTeleported = false;
					UpdateFloorFromAdjustment();
				}
			}
			else if (bForceNextFloorCheck)
			{
				UpdateFloorFromAdjustment();
			}
		}

		if (MovementMode != MOVE_None)
		{
			//TODO: Also ApplyAccumulatedForces()?
			HandlePendingLaunch();
		}
		ClearAccumulatedForces();

		if (MovementMode == MOVE_None)
		{
			return;
		}

		const bool bSimGravityDisabled = (bIsSimulatedProxy && CharacterOwner->bSimGravityDisabled);
		const bool bZeroReplicatedGroundVelocity = (bIsSimulatedProxy && IsMovingOnGround() && ConstRepMovement.LinearVelocity.IsZero());

		// bSimGravityDisabled means velocity was zero when replicated and we were stuck in something. Avoid external changes in velocity as well.
		// Being in ground movement with zero velocity, we cannot simulate proxy velocities safely because we might not get any further updates from the server.
		if (bSimGravityDisabled || bZeroReplicatedGroundVelocity)
		{
			Velocity = FVector::ZeroVector;
		}

		MaybeUpdateBasedMovement(DeltaSeconds);

		// simulated pawns predict location
		OldVelocity = Velocity;
		OldLocation = UpdatedComponent->GetComponentLocation();

		UpdateProxyAcceleration();

		// May only need to simulate forward on frames where we haven't just received a new position update.
		if (!bHandledNetUpdate || !bNetworkSkipProxyPredictionOnNetUpdate || !CharacterMovementCVars::NetEnableSkipProxyPredictionOnNetUpdate)
		{
			UE_LOG(LogCharacterMovement, Verbose, TEXT("Proxy %s simulating movement"), *GetNameSafe(CharacterOwner));
			FStepDownResult StepDownResult;
			MoveSmooth(Velocity, DeltaSeconds, &StepDownResult);

			// find floor and check if falling
			if (IsMovingOnGround() || MovementMode == MOVE_Falling)
			{
				if (StepDownResult.bComputedFloor)
				{
					CurrentFloor = StepDownResult.FloorResult;
				}
				else if (Velocity.Z <= 0.f)
				{
					FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, Velocity.IsZero(), NULL);
				}
				else
				{
					CurrentFloor.Clear();
				}

				if (!CurrentFloor.IsWalkableFloor())
				{
					if (!bSimGravityDisabled)
					{
						// No floor, must fall.
						if (Velocity.Z <= 0.f || bApplyGravityWhileJumping || !CharacterOwner->IsJumpProvidingForce())
						{
							Velocity = NewFallVelocity(Velocity, FVector(0.f, 0.f, GetGravityZ()), DeltaSeconds);
						}
					}
					SetMovementMode(MOVE_Falling);
				}
				else
				{
					// Walkable floor
					if (IsMovingOnGround())
					{
						AdjustFloorHeight();
						SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
					}
					else if (MovementMode == MOVE_Falling)
					{
						if (CurrentFloor.FloorDist <= MIN_FLOOR_DIST || (bSimGravityDisabled && CurrentFloor.FloorDist <= MAX_FLOOR_DIST))
						{
							// Landed
							SetPostLandedPhysics(CurrentFloor.HitResult);
						}
						else
						{
							if (!bSimGravityDisabled)
							{
								// Continue falling.
								Velocity = NewFallVelocity(Velocity, FVector(0.f, 0.f, GetGravityZ()), DeltaSeconds);
							}
							CurrentFloor.Clear();
						}
					}
				}
			}
		}
		else
		{
			UE_LOG(LogCharacterMovement, Verbose, TEXT("Proxy %s SKIPPING simulate movement"), *GetNameSafe(CharacterOwner));
		}

		// consume path following requested velocity
		bHasRequestedVelocity = false;

		OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	} // End scoped movement update

	// Call custom post-movement events. These happen after the scoped movement completes in case the events want to use the current state of overlaps etc.
	CallMovementUpdateDelegate(DeltaSeconds, OldLocation, OldVelocity);

	SaveBaseLocation();
*/
	UpdateComponentVelocity();
	bJustTeleported = false;

	LastUpdateLocation = UpdatedComponent ? UpdatedComponent->GetComponentLocation() : FVector::ZeroVector;
	LastUpdateRotation = UpdatedComponent ? UpdatedComponent->GetComponentQuat() : FQuat::Identity;
	LastUpdateVelocity = Velocity;
}

void UOWSPawnMovementComponent::SmoothClientPosition(float DeltaSeconds)
{
	if (!HasValidData())
	{
		return;
	}

	// We shouldn't be running this on a server that is not a listen server.
	checkSlow(GetNetMode() != NM_DedicatedServer);
	checkSlow(GetNetMode() != NM_Standalone);

	// Only client proxies or remote clients on a listen server should run this code.
	const bool bIsSimulatedProxy = (OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_SimulatedProxy);
	const bool bIsRemoteAutoProxy = (OWSPawnWithAbilitiesOwner->GetRemoteRole() == ROLE_AutonomousProxy);
	if (!ensure(bIsSimulatedProxy || bIsRemoteAutoProxy))
	{
		return;
	}

	//SmoothClientPosition_Interpolate(DeltaSeconds);
	SmoothClientPosition_UpdateVisuals();
}

void UOWSPawnMovementComponent::SmoothClientPosition_UpdateVisuals()
{
	//SCOPE_CYCLE_COUNTER(STAT_CharacterMovementSmoothClientPosition_Visual);
	/*
	FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
	USkeletalMeshComponent* Mesh = OWSPawnWithAbilitiesOwner->GetMesh();
	if (ClientData && Mesh && !Mesh->IsSimulatingPhysics())
	{
		// Adjust mesh location and rotation
		const FVector NewRelTranslation = UpdatedComponent->GetComponentToWorld().InverseTransformVectorNoScale(ClientData->MeshTranslationOffset) + OWSPawnWithAbilitiesOwner->GetBaseTranslationOffset();
		const FQuat NewRelRotation = ClientData->MeshRotationOffset * OWSPawnWithAbilitiesOwner->GetBaseRotationOffset();
		Mesh->SetRelativeLocationAndRotation(NewRelTranslation, NewRelRotation, false, nullptr, GetTeleportType());
	}
	*/
}

void UOWSPawnMovementComponent::FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const
{
	//SCOPE_CYCLE_COUNTER(STAT_CharFindFloor);

	// No collision, no floor...
	if (!HasValidData() || !UpdatedComponent->IsQueryCollisionEnabled())
	{
		OutFloorResult.Clear();
		return;
	}

	//UE_LOG(OWS, VeryVerbose, TEXT("[Role:%d] FindFloor: %s at location %s"), (int32)OWSPawnWithAbilitiesOwner->GetLocalRole(), *GetNameSafe(OWSPawnWithAbilitiesOwner), *CapsuleLocation.ToString());
	check(OWSPawnWithAbilitiesOwner->GetCapsuleComponent());

	// Increase height check slightly if walking, to prevent floor height adjustment from later invalidating the floor result.
	const float HeightCheckAdjust = (IsMovingOnGround() ? MAX_FLOOR_DIST + KINDA_SMALL_NUMBER : -MAX_FLOOR_DIST);

	float FloorSweepTraceDist = FMath::Max(MAX_FLOOR_DIST, MaxStepHeight + HeightCheckAdjust);
	float FloorLineTraceDist = FloorSweepTraceDist;
	bool bNeedToValidateFloor = true;

	// Sweep floor
	if (FloorLineTraceDist > 0.f || FloorSweepTraceDist > 0.f)
	{
		UOWSPawnMovementComponent* MutableThis = const_cast<UOWSPawnMovementComponent*>(this);

		if (bAlwaysCheckFloor || !bCanUseCachedLocation || bForceNextFloorCheck || bJustTeleported)
		{
			MutableThis->bForceNextFloorCheck = false;
			//ComputeFloorDist(CapsuleLocation, FloorLineTraceDist, FloorSweepTraceDist, OutFloorResult, OWSPawnWithAbilitiesOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), DownwardSweepResult);
		}
		else
		{
			// Force floor check if base has collision disabled or if it does not block us.
			UPrimitiveComponent* MovementBase = OWSPawnWithAbilitiesOwner->GetMovementBase();
			const AActor* BaseActor = MovementBase ? MovementBase->GetOwner() : NULL;
			const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();

			if (MovementBase != NULL)
			{
				MutableThis->bForceNextFloorCheck = !MovementBase->IsQueryCollisionEnabled()
					|| MovementBase->GetCollisionResponseToChannel(CollisionChannel) != ECR_Block
					|| MovementBaseUtility::IsDynamicBase(MovementBase);
			}

			const bool IsActorBasePendingKill = BaseActor && BaseActor->IsPendingKill();

			if (!bForceNextFloorCheck && !IsActorBasePendingKill && MovementBase)
			{
				//UE_LOG(LogCharacterMovement, Log, TEXT("%s SKIP check for floor"), *CharacterOwner->GetName());
				OutFloorResult = CurrentFloor;
				bNeedToValidateFloor = false;
			}
			else
			{
				MutableThis->bForceNextFloorCheck = false;
				//ComputeFloorDist(CapsuleLocation, FloorLineTraceDist, FloorSweepTraceDist, OutFloorResult, OWSPawnWithAbilitiesOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), DownwardSweepResult);
			}
		}
	}

	// OutFloorResult.HitResult is now the result of the vertical floor check.
	// See if we should try to "perch" at this location.
	if (bNeedToValidateFloor && OutFloorResult.bBlockingHit && !OutFloorResult.bLineTrace)
	{
		const bool bCheckRadius = true;
		/*if (ShouldComputePerchResult(OutFloorResult.HitResult, bCheckRadius))
		{
			float MaxPerchFloorDist = FMath::Max(MAX_FLOOR_DIST, MaxStepHeight + HeightCheckAdjust);
			if (IsMovingOnGround())
			{
				MaxPerchFloorDist += FMath::Max(0.f, PerchAdditionalHeight);
			}

			FFindFloorResult PerchFloorResult;
			if (ComputePerchResult(GetValidPerchRadius(), OutFloorResult.HitResult, MaxPerchFloorDist, PerchFloorResult))
			{
				// Don't allow the floor distance adjustment to push us up too high, or we will move beyond the perch distance and fall next time.
				const float AvgFloorDist = (MIN_FLOOR_DIST + MAX_FLOOR_DIST) * 0.5f;
				const float MoveUpDist = (AvgFloorDist - OutFloorResult.FloorDist);
				if (MoveUpDist + PerchFloorResult.FloorDist >= MaxPerchFloorDist)
				{
					OutFloorResult.FloorDist = AvgFloorDist;
				}

				// If the regular capsule is on an unwalkable surface but the perched one would allow us to stand, override the normal to be one that is walkable.
				if (!OutFloorResult.bWalkableFloor)
				{
					OutFloorResult.SetFromLineTrace(PerchFloorResult.HitResult, OutFloorResult.FloorDist, FMath::Min(PerchFloorResult.FloorDist, PerchFloorResult.LineDist), true);
				}
			}
			else
			{
				// We had no floor (or an invalid one because it was unwalkable), and couldn't perch here, so invalidate floor (which will cause us to start falling).
				OutFloorResult.bWalkableFloor = false;
			}
		}*/
	}
}


/*
void UOWSPawnMovementComponent::AdjustProxyCapsuleSize()
{
	if (bShrinkProxyCapsule && OWSPawnWithAbilitiesOwner && OWSPawnWithAbilitiesOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		bShrinkProxyCapsule = false;

		float ShrinkRadius = FMath::Max(0.f, NetProxyShrinkRadius);
		float ShrinkHalfHeight = FMath::Max(0.f, NetProxyShrinkHalfHeight);

		if (ShrinkRadius == 0.f && ShrinkHalfHeight == 0.f)
		{
			return;
		}

		float Radius, HalfHeight;
		CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleSize(Radius, HalfHeight);
		const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();

		if (ComponentScale <= KINDA_SMALL_NUMBER)
		{
			return;
		}

		const float NewRadius = FMath::Max(0.f, Radius - ShrinkRadius / ComponentScale);
		const float NewHalfHeight = FMath::Max(0.f, HalfHeight - ShrinkHalfHeight / ComponentScale);

		if (NewRadius == 0.f || NewHalfHeight == 0.f)
		{
			UE_LOG(OWS, Warning, TEXT("Invalid attempt to shrink Proxy capsule for %s to zero dimension!"), *OWSPawnWithAbilitiesOwner->GetName());
			return;
		}

		UE_LOG(OWS, Verbose, TEXT("Shrinking capsule for %s from (r=%.3f, h=%.3f) to (r=%.3f, h=%.3f)"), *OWSPawnWithAbilitiesOwner->GetName(),
			Radius * ComponentScale, HalfHeight * ComponentScale, NewRadius * ComponentScale, NewHalfHeight * ComponentScale);

		OWSPawnWithAbilitiesOwner->GetCapsuleComponent()->SetCapsuleSize(NewRadius, NewHalfHeight, true);
	}
}
*/