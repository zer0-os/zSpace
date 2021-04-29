// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "JsonGlobals.h"
#include "Policies/JsonPrintPolicy.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonTypes.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JsonObjectConverter.h"
#include "EtherlinkerTypes.h"
#include "EtherlinkerSettings.h"
#include "EtherlinkerRemoteWalletManager.generated.h"

class UEtherlinkerSettings;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETHERLINKER_API UEtherlinkerRemoteWalletManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEtherlinkerRemoteWalletManager();

	/**
	 * Event dispatcher - OnResponseReceivedEvent
	 */
	UPROPERTY(BlueprintAssignable)
	FWalletAuthenticationResponseReceivedDelegate OnResponseReceivedEvent;

	/**
	 * Get wallet data by login/password
	 *
	 * @param walletAuthenticationRequest request data to send to the Integration Server
	 */
	UFUNCTION(BlueprintCallable, Category = "EtherlinkerRemoteWalletManager")
	void GetWalletData(FWalletAuthenticationRequest walletAuthenticationRequest);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerGetWalletData(FWalletAuthenticationRequest walletAuthenticationRequest);

	void ServerGetWalletData_Implementation(FWalletAuthenticationRequest walletAuthenticationRequest);

	bool ServerGetWalletData_Validate(FWalletAuthenticationRequest walletAuthenticationRequest);

	/**
	 * Create new user account and wallet for it and store everything in database
	 *
	 * @param walletAuthenticationRequest request data to send to the Integration Server
	 */
	UFUNCTION(BlueprintCallable, Category = "EtherlinkerRemoteWalletManager")
	void CreateUserAccount(FWalletAuthenticationRequest walletAuthenticationRequest);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCreateUserAccount(FWalletAuthenticationRequest walletAuthenticationRequest);

	void ServerCreateUserAccount_Implementation(FWalletAuthenticationRequest walletAuthenticationRequest);

	bool ServerCreateUserAccount_Validate(FWalletAuthenticationRequest walletAuthenticationRequest);
	
private:

	/**
	 * Make a JSON request over HTTP
	 *
	 * @param WalletAuthenticationRequest request data to send to the Integration Server
	 * @param URL MVC controller method address on integration server 
	 * @return true if request successfully sent
	 */
	bool MakeRequest(FWalletAuthenticationRequest WalletAuthenticationRequest, const FString& URL);

	/**
	 * Receive response for JSON request
	 *
	 * @param Request request data, which was sent to the integration server
	 * @param Response response from the integration server
	 * @param bWasSuccessful true if response successfully processed on the integration server
	 */
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};