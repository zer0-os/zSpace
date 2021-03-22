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

	AZPPlayerController();
	
	virtual  void BeginPlay() override;

	virtual  void Tick(float NewDeltaSeconds) override;

	virtual void SetupInputComponent() override;	
	
};

