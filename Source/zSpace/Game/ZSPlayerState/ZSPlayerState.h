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

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherManager * ZSEtherManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSEtherlinkerRemoteWalletManager * ZSEtherlinkerRemoteWalletManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	int32 UserIndex = -1;

	friend class UZSEtherlinkerRemoteWalletManager;
	
};
