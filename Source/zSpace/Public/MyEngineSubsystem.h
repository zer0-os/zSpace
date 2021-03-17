// Copyright 2018 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MyEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UMyEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
