// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "zSpace/EthereumTerminalComponent/EthereumTerminalComponent.h"
#include "EthereumTerminalDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UEthereumTerminalDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FZSEtherlinkerRequestData ZSEtherlinkerRequestData;
	
public:

	UFUNCTION(BlueprintCallable)
	FZSEtherlinkerRequestData GetZSEtherlinkerRequestData();
	
};
