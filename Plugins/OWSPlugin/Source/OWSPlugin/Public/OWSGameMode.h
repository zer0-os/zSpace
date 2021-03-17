// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "OWSCharacter.h"
#include "OWSPlayerController.h"
#include "OWSGameMode.generated.h"

USTRUCT(BlueprintType)
struct FZoneInstanceStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Instance Struct")
		int32 MapInstanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Instance Struct")
		int32 Port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Instance Struct")
		int32 Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Instance Struct")
		FString ZoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Instance Struct")
		FString MapName;


};

USTRUCT(BlueprintType)
struct FCharactersOnlineStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString CharName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 Alignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 Fame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 TeamNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FGuid UserGUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString CreateDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString LoginDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FGuid UserSessionGUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 MapInstanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 MapID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		int32 WorldServerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString Port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString ZoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters Online Struct")
		FString MapName;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemLibraryLoadedSignature);
/**
 * 
 */
UCLASS()
class OWSPLUGIN_API AOWSGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:

	FHttpModule* Http;

	int NextSaveGroupIndex = -1;

	FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal);	

public:

	AOWSGameMode();

	virtual void StartPlay();

	APawn * SpawnDefaultPawnFor_Implementation(AController * NewPlayer, class AActor * StartSpot);

	UPROPERTY(BlueprintAssignable, Category = "Item Library Loaded")
		FItemLibraryLoadedSignature ItemLibraryLoadedEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		FVector DebugStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		FString DebugCharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		TArray<FInventoryItemStruct> AllInventoryItems;

	UPROPERTY()
		TMap<FString, int32> MeshItemsMap;

	UFUNCTION()
		void AddItemMeshToAllPlayers(const FString& ItemName, const int32 ItemMeshID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FInventoryItemStruct& FindItemDefinition(FString ItemName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zones")
		TArray<FCharactersOnlineStruct> CharactersOnline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zones")
		FString IAmZoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float GetCharactersOnlineIntervalInSeconds = 10.f;

	FTimerHandle OnGetAllCharactersOnlineTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zones")
		float UpdateServerStatusEveryXSeconds = 10.f;

	FTimerHandle UpdateServerStatusEveryXSecondsTimerHandle;


	//Time of Day
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeOfDay")
		float LocalSecondsOffset; //The time offset when this instance started

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeOfDay")
		float DayLengthInSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeOfDay")
		float DaysPerLunarCycle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeOfDay")
		float DaysPerSolarCycle;



	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPICustomerKey;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWSEncryptionKey = "";
	
	//Get All Inventory Items
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void GetAllInventoryItems();

	void OnGetAllInventoryItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void NotifyGetAllInventoryItems();
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void ErrorGetAllInventoryItems(const FString &ErrorMsg);

	//Get Global Data Item
	UFUNCTION(BlueprintCallable, Category = "Global Data")
		void GetGlobalDataItem(FString GlobalDataKey);

	void OnGetGlobalDataItemResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Global Data")
		void NotifyGetGlobalDataItem(const FString &GlobalDataKey);
	UFUNCTION(BlueprintImplementableEvent, Category = "Global Data")
		void ErrorGetGlobalDataItem(const FString &ErrorMsg);

	//Add or Update Global Data Item
	UFUNCTION(BlueprintCallable, Category = "Global Data")
		void AddOrUpdateGlobalDataItem(FString GlobalDataKey, FString GlobalDataValue);

	void OnAddOrUpdateGlobalDataItemResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Global Data")
		void NotifyAddOrUpdateGlobalDataItem();
	UFUNCTION(BlueprintImplementableEvent, Category = "Global Data")
		void ErrorAddOrUpdateGlobalDataItem(const FString &ErrorMsg);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		float SaveIntervalInSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		int SplitSaveIntoHowManyGroups;

	//Save all player locations
	UFUNCTION(BlueprintCallable, Category = "Character")
		void SaveAllPlayerLocations();

	void OnSaveAllPlayerLocationsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Get all players online
	UFUNCTION(BlueprintCallable, Category = "Character")
		void GetAllCharactersOnline();

	void OnGetAllCharactersOnlineResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*UFUNCTION(BlueprintImplementableEvent, Category = "Character")
		void NotifyGetAllCharactersOnline(const TArray<FCharactersOnlineStruct> &CharactersOnline);*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
		void ErrorGetAllCharactersOnline(const FString &ErrorMsg);

	//Is player online
	UFUNCTION(BlueprintCallable, Category = "Character")
		bool IsPlayerOnline(FString CharacterName);

	//Get all running zone instances for a Zone
	UFUNCTION(BlueprintCallable, Category = "Zones")
		void GetZoneInstancesForZone(FString ZoneName);

	void OnGetZoneInstancesForZoneResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void NotifyGetZoneInstancesForZone(const TArray<FZoneInstanceStruct> &ZoneInstances);
	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void ErrorGetZoneInstancesForZone(const FString &ErrorMsg);

	//Lookup a zone instance from a given port
	UFUNCTION(BlueprintCallable, Category = "Zones")
		void GetServerInstanceFromPort();

	void OnGetServerInstanceFromPortResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void NotifyGetServerInstanceFromPort(const FString &ZoneName);
	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void ErrorGetServerInstanceFromPort(const FString &ErrorMsg);

	//Update Number of Players
	UFUNCTION(BlueprintCallable, Category = "Zones")
	void UpdateNumberOfPlayers();

	void OnUpdateNumberOfPlayersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Zones")
		FString GetAddressURLAndPort();


	//Get Current World Time
	//Lookup a zone instance from a given port
	UFUNCTION(BlueprintCallable, Category = "Zones")
		void GetCurrentWorldTime();

	void OnGetCurrentWorldTimeResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void NotifyGetCurrentWorldTime(const float CurrentWorldTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Zones")
		void ErrorGetCurrentWorldTime(const FString &ErrorMsg);

	AOWSPlayerController* GetPlayerControllerFromCharacterName(const FString CharacterName);

protected:
	void BroadcastItemLibraryLoaded()
	{
		ItemLibraryLoadedEvent.Broadcast();
	}
	
};
