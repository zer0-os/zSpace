// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSPlayerController.h"
#include "ZSPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ZSPACE_API AZSPlayerController : public AOWSPlayerController
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscButtonPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetAllCharacters, const TArray<FUserCharacter>&, UserCharacters);

	AZSPlayerController();

	virtual  void BeginPlay() override;

	virtual  void Tick(float NewDeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
		void OnEscOnClicked();

	// void NotifyGetAllCharacters_Implementation(const TArray<FUserCharacter> &UserCharacters);

public:
	UPROPERTY(BlueprintAssignable)
		FOnEscButtonPressed OnEscButtonPressed;
	
	UPROPERTY(BlueprintAssignable)
		FOnGetAllCharacters OnGetAllCharacters;
};

