// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "UObject/Interface.h"
#include "ZSEthRemoteWalletInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZSEthRemoteWalletInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IZSEthRemoteWalletInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEtherResponseReceived(const FString & NewResult, const FEtherlinkerResponseData & NewData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateBindings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnRWAResponseReceived(const FString & NewResult, const FEtherlinkerResponseData & NewData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEtherBatchResponseReceived(const FString & NewResult, const FEtherlinkerResponseData & NewData);
	
};
