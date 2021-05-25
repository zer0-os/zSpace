// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "EtherManager.h"
#include "ZSEtherManager.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSEtherManager : public UEtherManager
{
	GENERATED_BODY()
	
public:
	
	UZSEtherManager();


	UFUNCTION(Client, Reliable)
	void Client_ResponseReceived(const FString & Result, const FEtherlinkerResponseData & Data);

	void Server_ResponseReceived(FString  Result, FEtherlinkerResponseData  Data);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FClientResponseReceivedEvent, FString,  Result, FEtherlinkerResponseData, Data);

	UPROPERTY(BlueprintAssignable)
	FClientResponseReceivedEvent OnClientResponseReceivedEvent;

};
