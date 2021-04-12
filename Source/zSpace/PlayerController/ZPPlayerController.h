// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSPlayerController.h"
#include "ZPPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ZSPACE_API AZPPlayerController : public AOWSPlayerController
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscButtonPressed);

	AZPPlayerController();

	virtual  void BeginPlay() override;

	virtual  void Tick(float NewDeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
		void OnEscOnClicked();

public:

	UPROPERTY(BlueprintAssignable)
		FOnEscButtonPressed OnEscButtonPressed;
};

