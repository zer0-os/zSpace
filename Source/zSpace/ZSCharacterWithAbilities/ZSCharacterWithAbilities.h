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

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float BaseTurnRate = 45;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	float BaseLookUpRate = 45;

public:

	void Turn(float NewValue);

	void LookUp(float NewValue);

	virtual void Jump() override;

	virtual void StopJumping() override;

	void MoveForward(float NewValue);

	void MoveRight(float NewValue);

	void Dodge();

	///  Show a List of all players online across all instances.
	void ShowPlayersOnline();

	///  Show a List of all players online across all instances.
	UFUNCTION(Server, WithValidation, Unreliable)
	void Server_ShowPlayersOnline();
	
};



