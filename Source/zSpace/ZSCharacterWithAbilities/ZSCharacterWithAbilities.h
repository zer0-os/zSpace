// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacterWithAbilities.h"
#include "AnimInstances/ZSAnimationTypes.h"

#include "ZSCharacterWithAbilities.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSCharacterWithAbilities : public AOWSCharacterWithAbilities
{
	GENERATED_BODY()

	friend class UZSCharacterMovementComponent;

public:

	AZSCharacterWithAbilities(const FObjectInitializer & NewObjectInitializer);


	virtual void SetupPlayerInputComponent(UInputComponent* NewPlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	uint8 bIsWalking : 1;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	uint8 bIsMoveInputPressed : 1;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveForwardAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float LastMoveForwardAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveRightAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float LastMoveRightAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float CharacterRelativeRotation = 0.f;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Movement")
	float MoveInputKeyTimeDownAverage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Animations|Monateg")
	class UAnimMontage* StopMovementAnimMontageRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations|Monateg")
	class UAnimMontage* StopMovementAnimMontageLeft = nullptr;

	// [Server]
	UPROPERTY(Transient)
	class UAnimMontage* CurrentPlayingStopMovementAnimMontage = nullptr;
	
	// [Server]
	FTimerHandle MoveInputKeyTimeDownAverage_TimerHandle;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsMoveInputPressed() const { return bIsMoveInputPressed; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveForwardAxisValue() const { return  MoveForwardAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetLastMoveForwardAxisValue() const { return  LastMoveForwardAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveRightAxisValue() const { return  MoveRightAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetLastMoveRightAxisValue() const { return LastMoveRightAxisValue; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCharacterRelativeRotation() const { return CharacterRelativeRotation; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMoveInputKeyTimeDownAverage() const { return MoveInputKeyTimeDownAverage; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_PlayMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool PlayInServer = false);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMulticast_PlayMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool PlayInServer = false);

	UFUNCTION(BlueprintPure)
	bool IsStopMovementAnimMontagePlaying() const;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StopMontage(float InBlendOutTime, const UAnimMontage* Montage);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMulticast_StopMontage(float InBlendOutTime, const UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
	class UAnimMontage* PlayStopMovementAnimMontage();
	
protected:
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
};
