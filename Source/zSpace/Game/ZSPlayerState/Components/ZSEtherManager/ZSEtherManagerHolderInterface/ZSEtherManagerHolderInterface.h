// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZSEtherManagerHolderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZSEtherManagerHolderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IZSEtherManagerHolderInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecuteUpdateBindings(class UActorComponent * NewEtherInteractor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UZSEtherlinkerRemoteWalletManager * GetEtherlinkerRemoteWalletManager();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int32 GetUserIndex();
	
};
