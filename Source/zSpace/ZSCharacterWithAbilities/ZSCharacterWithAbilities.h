// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacterWithAbilities.h"
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
	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 bIsWalking : 1;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 bIsMoveInputPressed : 1;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float MoveForwardAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float LastMoveForwardAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float MoveRightAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float LastMoveRightAxisValue;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float CharacterRelativeRotation = 0.f;

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

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsWalking(bool NewValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsMoveInputPressed(bool NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMoveForwardAxisValue(const float& NewValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMoveRightAxisValue(const float& NewValue);

	// [Server]
	float CalculateCharacterRelativeRotation() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
