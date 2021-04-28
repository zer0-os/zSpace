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

public:

	AZSCharacterWithAbilities(const FObjectInitializer & NewObjectInitializer);


	virtual void SetupPlayerInputComponent(UInputComponent* NewPlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float NewDeltaSeconds) override;

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

	
	
};



