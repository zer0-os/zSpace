// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSGameMode.h"
#include "ZSEtherlinkerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSEtherlinkerGameMode : public AOWSGameMode
{
	GENERATED_BODY()

public:

	AZSEtherlinkerGameMode();

	virtual void BeginPlay() override;

	void StartIntegrationServer();	
	
};


