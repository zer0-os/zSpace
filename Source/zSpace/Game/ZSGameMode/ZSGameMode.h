// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSGameMode.h"
#include "ZSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSGameMode : public AOWSGameMode
{
	GENERATED_BODY()
	
	virtual APawn * SpawnDefaultPawnFor_Implementation(AController * NewPlayer, class AActor * StartSpot);

};
