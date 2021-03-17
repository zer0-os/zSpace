// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSCharacter.h"
#include "OWSPlayerState.h"
#include "OWSPlayerControllerComponent.generated.h"

struct FChatGroup;

//Get Chat Groups for Player Delegates
DECLARE_DELEGATE_OneParam(FNotifyGetChatGroupsForPlayerDelegate, const TArray<FChatGroup>&)
DECLARE_DELEGATE_OneParam(FErrorGetChatGroupsForPlayerDelegate, const FString&)



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OWSPLUGIN_API UOWSPlayerControllerComponent : public UActorComponent
{
	GENERATED_BODY()

	FHttpModule* Http;

public:	
	// Sets default values for this component's properties
	UOWSPlayerControllerComponent();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		AOWSPlayerState* GetOWSPlayerState() const;

	//Travel methods
	UFUNCTION(BlueprintCallable, Category = "Travel")
		void TravelToMap(const FString& URL, const bool SeamlessTravel);

	UFUNCTION(BlueprintCallable, Category = "Travel")
		void TravelToMap2(const FString& ServerAndPort, const float X, const float Y, const float Z, const float RX, const float RY,
			const float RZ, const FString& PlayerName, const bool SeamlessTravel);

	UFUNCTION(BlueprintCallable, Category = "Travel")
		void SetSelectedCharacterAndConnectToLastZone(FString UserSessionGUID, FString SelectedCharacterName);

	void OnSetSelectedCharacterAndConnectToLastZoneResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void TravelToLastZoneServer(FString CharacterName);
	void OnTravelToLastZoneServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Save")
		void SavePlayerLocation();

	void OnSavePlayerLocationResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Save")
		void SaveAllPlayerData();

	void OnSaveAllPlayerDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Get Chat Groups for Player
	UFUNCTION(BlueprintCallable, Category = "Chat")
		void GetChatGroupsForPlayer();

	void OnGetChatGroupsForPlayerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FNotifyGetChatGroupsForPlayerDelegate OnNotifyGetChatGroupsForPlayerDelegate;
	FErrorGetChatGroupsForPlayerDelegate OnErrorGetChatGroupsForPlayerDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetPlayerNameAndOWSCharacter(AOWSCharacter* OWSCharacter, FString& PlayerName);

	UPROPERTY(BlueprintReadWrite)
		FString RPGAPICustomerKey;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWS2APIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWSEncryptionKey = "";

	UPROPERTY(BlueprintReadWrite)
		float TravelTimeout = 60.f;

	FString ServerTravelUserSessionGUID;
	FString ServerTravelCharacterName;
	float ServerTravelX;
	float ServerTravelY;
	float ServerTravelZ;
	float ServerTravelRX;
	float ServerTravelRY;
	float ServerTravelRZ;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
