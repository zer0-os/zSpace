// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "OWSPlayerState.h"
#include "ZSPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class ZSPACE_API AZSPlayerState : public AOWSPlayerState
{
	GENERATED_BODY()
	
public:
	
	AZSPlayerState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;



};
