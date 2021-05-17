// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZSEthereumActorCompInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZSEthereumActorCompInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IZSEthereumActorCompInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CreateRemoteWalletFromUI(const FString & NewLogin, const FString & NewPassword);

	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FString GetSenderID();
	
};
