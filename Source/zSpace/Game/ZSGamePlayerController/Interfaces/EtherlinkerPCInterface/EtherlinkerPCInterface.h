// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EtherlinkerPCInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEtherlinkerPCInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IEtherlinkerPCInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UZSEtherlinkerRemoteWalletManager *  GetZSEtherlinkerRemoteWalletManager();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UZSEtherManager * GetZSEtherManager();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UZSEthereumActorComponent	* GetZSEthereumActorComponent();
	
	
};
