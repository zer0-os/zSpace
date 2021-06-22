// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacterWithAbilities.h"
#include "AnimInstances/ZSAnimationTypes.h"
#include "AnimInstances/ZSAnimInstance.h"

#include "ZSCharacterWithAbilities.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSCharacterWithAbilities : public AOWSCharacterWithAbilities
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeAnimationState, const EAnimationState, AnimationState);
	
	friend class UZSCharacterMovementComponent;

public:

	AZSCharacterWithAbilities(const FObjectInitializer & NewObjectInitializer);


	virtual void SetupPlayerInputComponent(UInputComponent* NewPlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupOculusSettings();

	virtual void Tick(float NewDeltaSeconds) override;

	virtual bool CanCrouch() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float BaseTurnRate = 45;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float BaseLookUpRate = 45;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class USpringArmComponent * SpringArmComponent = nullptr; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class UCameraComponent * CameraComponent =  nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class UCameraComponent * CameraComponentVR =  nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class USceneComponent * SceneComponentVR = nullptr;	
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class UDetectSurfaceTypeComponent * DetectSurfaceTypeComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftObjectPtr<class USoundBase> SoundBaseAcceleration;

public:
	UFUNCTION()
	void Turn(float NewValue);

	UFUNCTION()
	void LookUp(float NewValue);

	void JumpIntoWater();
	
	virtual void Jump() override;

	virtual void StopJumping() override;

	UFUNCTION()
	void MoveForward(float NewValue);

	UFUNCTION()
	void MoveRight(float NewValue);

	UFUNCTION()
	void Dodge();

	///  Show a List of all players online across all instances.
	UFUNCTION()
	void ShowPlayersOnline();

	///  Show a List of all players online across all instances.
	UFUNCTION(Server, WithValidation, Unreliable)
	void Server_ShowPlayersOnline();

	UFUNCTION()
	void OnStartWalking();
	
	UFUNCTION()
	void OnStopWalking();

	UFUNCTION()
	void OnStartCrouching();
	
	UFUNCTION()
	void OnStopCrouching();

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Movement")
	uint8 bIsDeath : 1;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	uint8 bIsWalking : 1;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	uint8 bIsMoveInputPressed : 1;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveForwardAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveRightAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float CharacterRelativeRotation = 0.f;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveInputKeyTimeDownAverage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Animations|Monateg")
	class UAnimMontageLocomotionDataAsset* StopMovementAnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations|Monateg")
	class UAnimMontageLocomotionDataAsset* StartMovementAnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations|Monateg")
	class UAnimMontage* AttackMontage = nullptr;

	// [Server]
	FTimerHandle MoveInputKeyTimeDownAverage_TimerHandle;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AnimationState)
	EAnimationState AnimationState;

	UFUNCTION()
	void OnRep_AnimationState();

	// Server
	float StartControlRotationYaw = 0.f;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeAnimationState OnChangeAnimationState;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsMoveInputPressed() const { return bIsMoveInputPressed; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveForwardAxisValue() const { return  MoveForwardAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveRightAxisValue() const { return  MoveRightAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCharacterRelativeRotation() const { return CharacterRelativeRotation; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveInputKeyTimeDownAverage() const { return MoveInputKeyTimeDownAverage; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsDeath() const { return bIsDeath; }
	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_PlayMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool PlayInServer = false);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMulticast_PlayMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool PlayInServer = false);

	UFUNCTION(BlueprintPure)
	bool IsStopMovementAnimMontagePlaying() const;
	
	UFUNCTION(BlueprintPure)
	bool IsStartMovementAnimMontagePlaying() const;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StopMontage(float InBlendOutTime, const UAnimMontage* Montage);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMulticast_StopMontage(float InBlendOutTime, const UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
	class UAnimMontage* PlayStopMovementAnimMontage();

	UFUNCTION(BlueprintCallable)
	class UAnimMontage* PlayStartMovementAnimMontage();
	
	UFUNCTION(BlueprintCallable)
	void StopStopMovementAnimMontage();

	UFUNCTION(BlueprintCallable)
	void StopStartMovementAnimMontage(float InBlendOutTime = 0.25);
	
	UFUNCTION(BlueprintPure)
	class UZSCharacterMovementComponent* GetZSCharacterMovement() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAnimationState(const EAnimationState& NewValue);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE EAnimationState GetAnimationState() const { return AnimationState; }

	UFUNCTION(BlueprintPure)
	class UZSAnimInstance* GetAnimInstance() const;
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_OnChangeAnimationState(const EAnimationState& CurrentValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsWalking(bool NewValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsMoveInputPressed(bool NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMoveForwardAxisValue(const float& NewValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMoveRightAxisValue(const float& NewValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMoveInputKeyTimeDownAverage(const float& NewValue);

	UFUNCTION()
	void OnChangedPlayerGait(EPlayerGait CurrentPlayerGait);

	// [Server]
	float CalculateCharacterRelativeRotation() const;

	/**
		MoveForward
		MoveRight
	*/
	UFUNCTION(BlueprintPure)
	float CalculateMoveInputKeyTimeDownAverage() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UMotionControllerComponent * MotionControllerComponentLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UMotionControllerComponent * MotionControllerComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USkeletalMeshComponent * SkeletalMeshComponentLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USkeletalMeshComponent * SkeletalMeshComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponentLeft = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponentRight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UBallisticLineComponent * BallisticLineComponentLeft = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UBallisticLineComponent * BallisticLineComponentRight = nullptr;
		
public:

	UFUNCTION()
	void OculusLTeleportPressed();

	UFUNCTION()
	void OculusLTeleportReleased();
	
	UFUNCTION()
	void OculusRTeleportPressed();
	
	UFUNCTION()
	void OculusRTeleportReleased();
	
	UFUNCTION()
	void HoveredWidgetChanged(class UWidgetComponent* NewWidgetComponent, class UWidgetComponent* NewPreviousWidgetComponent);
	
};

