// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSCharacter.h"
#include "OWSPlayerState.h"
#include "OWSReplicationGraph.h"
#include "OWSPlayerControllerComponent.h"
#include "OWSPlayerController.generated.h"


UENUM(BlueprintType)
namespace ERPGPlayerGroupType
{
	enum PlayerGroupType
	{
		NoGroup UMETA(DisplayName = "NoGroup"),
		Party UMETA(DisplayName = "Party"),
		Raid UMETA(DisplayName = "Raid"),
		Guild UMETA(DisplayName = "Guild"),
		Team UMETA(DisplayName = "Team"),
		Faction UMETA(DisplayName = "Faction"),
		FlightGroup UMETA(DisplayName = "FlightGroup"),
		Alliance UMETA(DisplayName = "Alliance"),
		Squad UMETA(DisplayName = "Squad"),
		Other = 99
	};
}


USTRUCT(BlueprintType, Blueprintable)
struct FChatGroup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chat")
		int32 ChatGroupID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chat")
		FString ChatGroupName;
};


USTRUCT(BlueprintType, Blueprintable)
struct FPlayerGroup
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		int32 PlayerGroupID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		FString PlayerGroupName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		int32 PlayerGroupTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		FDateTime DateAdded;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		int32 ReadyState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Group")
		int32 TeamNumber;
};


USTRUCT(BlueprintType, Blueprintable)
struct FUserCharacter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString ClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Gender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString ZoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Silver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Copper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 FreeCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 PremiumCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 XP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString LastActivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString CreateDate;
	
};

USTRUCT(BlueprintType, Blueprintable)
struct FCreateCharacter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool Success;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString ErrorMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString ClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 CharacterLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString StartingMapName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float Z;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float RX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float RY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float RZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 TeamNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Silver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Copper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 FreeCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 PremiumCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Fame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Alignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Gender;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 XP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int32 Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString LastActivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString CreateDate;

};

/**
 * 
 */
UCLASS()
class OWSPLUGIN_API AOWSPlayerController : public APlayerController
{
	GENERATED_BODY()

	FHttpModule* Http;

public:

	AOWSPlayerController();

	UPROPERTY()
		UOWSPlayerControllerComponent* OWSPlayerControllerComponent;

	UPROPERTY(BlueprintReadWrite)
		FString RPGAPICustomerKey;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWS2APIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWSEncryptionKey = "";

	AOWSGameMode* GetGameMode();

	UFUNCTION(BlueprintCallable, Category = "Travel")
		void SetSelectedCharacterAndConnectToLastZone(FString UserSessionGUID, FString SelectedCharacterName);

		void OnSetSelectedCharacterAndConnectToLastZoneResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

		void TravelToLastZoneServer(FString CharacterName);

		void OnTravelToLastZoneServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

		FString ServerTravelUserSessionGUID;
		FString ServerTravelCharacterName;
		float ServerTravelX;
		float ServerTravelY;
		float ServerTravelZ;
		float ServerTravelRX;
		float ServerTravelRY;
		float ServerTravelRZ;

	FVector LastCharacterLocation;
	FRotator LastCharacterRotation;

	UPROPERTY()
		TMap<FString, int32> LocalMeshItemsMap;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SynchUpLocalMeshItemsMap();

	UFUNCTION()
		void AddItemToLocalMeshItemsMap(const FString& ItemName, const int32 ItemMeshID);

	UFUNCTION(Client, Reliable)
		void Client_AddItemToLocalMeshItemsMap(const FString& ItemName, const int32 ItemMeshID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
		AOWSCharacter* SelectedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groups")
		TArray<FPlayerGroup> PlayerGroupsPlayerIsIn;

	UFUNCTION(BlueprintCallable, Category = "Player State")
		AOWSPlayerState* GetOWSPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Replication")
		UOWSReplicationGraph* GetReplicationGraph() const;

	UFUNCTION(BlueprintCallable, Category = "Travel")
		void TravelToMap(const FString& URL, const bool SeamlessTravel);

	UFUNCTION(BlueprintCallable, Category = "Travel")
		void TravelToMap2(const FString& ServerAndPort, const float X, const float Y, const float Z, const float RX, const float RY,
			const float RZ, const FString& PlayerName, const bool SeamlessTravel);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection")
		float TravelTimeout;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	//Predicted projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		TArray<class AOWSAdvancedProjectile*> FakeProjectiles;

	/** Return amount of time to tick or simulate to make up for network lag */
	UFUNCTION(BlueprintCallable, Category = "Prediction")
	virtual float GetPredictionTime();

	virtual float GetProjectileSleepTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Projectiles")
		float MaxPredictionPing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		float DesiredPredictionPing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Projectiles")
		float PredictionFudgeFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		bool bIsDebuggingProjectiles;

	UFUNCTION(BlueprintCallable, Category = "Utility")
		UTexture2D* LoadTextureReference(const FString& TexturePath);

	UFUNCTION(BlueprintCallable, Category = "Selection")
		void SetSelectedCharacter(AOWSCharacter* RPGCharacter);
	UFUNCTION(BlueprintCallable, Category = "Selection")
		void ClearSelectedCharacter();
	UFUNCTION(BlueprintCallable, Category = "Selection")
		void ClearSelectionOnCharacter(AOWSCharacter* RPGCharacter);
	
	UFUNCTION(BlueprintCallable, Category = "Save")
		void SavePlayerLocation();

	void OnSavePlayerLocationResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Save")
		void SaveAllPlayerData();

	void OnSaveAllPlayerDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void PawnLeavingGame();

	//bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	bool InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Save")
		void NotifyPawnLeavingGame(const AOWSCharacter* RPGCharacter);

	//Add Player to Group
	UFUNCTION(BlueprintCallable, Category = "Groups")
		void AddPlayerToGroup(FString PlayerGroupName, ERPGPlayerGroupType::PlayerGroupType GroupType, FString CharacterNameToAdd);

	void OnAddPlayerToGroupResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Groups")
		void NotifyAddPlayerToGroup(const FString& CharacterNameAddedToGroup);

	//Remove Player from Group
	UFUNCTION(BlueprintCallable, Category = "Groups")
		void RemovePlayerFromGroup(FString PlayerGroupName, FString CharacterNameToRemove);

	void OnRemovePlayerFromGroupResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Groups")
		void NotifyRemovePlayerFromGroup(const FString& CharacterNameAddedToGroup);

	//Launch Dungeon
	UFUNCTION(BlueprintCallable, Category = "Travel")
		void LaunchDungeon(FString MapName, ERPGPlayerGroupType::PlayerGroupType GroupType);

	void OnLaunchDungeonResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Travel")
		void NotifyLaunchDungeon(const FString &ServerAndPort);
	UFUNCTION(BlueprintImplementableEvent, Category = "Travel")
		void ErrorLaunchDungeon(const FString &ErrorMsg);

	//Player Logout
	UFUNCTION(BlueprintCallable, Category = "Player")
		void PlayerLogout();

	void OnPlayerLogoutResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void NotifyPlayerLogout();
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void ErrorPlayerLogout(const FString &ErrorMsg);

	//Get Chat Groups for Player
	UFUNCTION(BlueprintCallable, Category = "Chat")
		void GetChatGroupsForPlayer();

	void OnGetChatGroupsForPlayerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
		void NotifyGetChatGroupsForPlayer(const TArray<FChatGroup> &ChatGroups);
	UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
		void ErrorGetChatGroupsForPlayer(const FString &ErrorMsg);

	//Is Player Online?
	UFUNCTION(BlueprintCallable, Category = "Chat")
		void IsPlayerOnline(FString PlayerName);

	void OnIsPlayerOnlineResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
		void NotifyIsPlayerOnline(const bool PlayerIsOnline);
	UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
		void ErrorIsPlayerOnline(const FString &ErrorMsg);

	//Get All Characters for UserSessionGUID
	UFUNCTION(BlueprintCallable, Category = "Login")
		void GetAllCharacters(FString UserSessionGUID);

	void OnGetAllCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyGetAllCharacters(const TArray<FUserCharacter> &UserCharacters);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorGetAllCharacters(const FString &ErrorMsg);

	//Create Character
	UFUNCTION(BlueprintCallable, Category = "Login")
		void CreateCharacter(FString UserSessionGUID, FString CharacterName, FString ClassName);

	void OnCreateCharacterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyCreateCharacter(const FCreateCharacter &CreateCharacterData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorCreateCharacter(const FString &ErrorMsg);

	//Get Last Zone Server
	UFUNCTION(BlueprintCallable, Category = "Travel")
		void GetLastZoneServerToTravelTo(FString CharacterName);

	void OnGetLastZoneServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Travel")
		void NotifyLastZoneServerToTravelTo(const FString &ServerAndPort);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Travel")
		void ErrorLastZoneServerToTravelTo(const FString &ErrorMsg);

	//Get User Session
	UFUNCTION(BlueprintCallable, Category = "Login")
		void GetUserSession(FString UserSessionGUID);

	void OnGetUserSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Login")
		void NotifyGetUserSession(const FString &CharacterName, const FString &Email, const FString &FirstName, const FString &LastName, const FString &ZoneName, 
			const float &X, const float &Y, const float &Z, const float &RX, const float &RY, const float &RZ);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Login")
		void ErrorGetUserSession(const FString &ErrorMsg);

	//UserSession Set Selected Character	
	UFUNCTION(BlueprintCallable, Category = "Login")
		void UserSessionSetSelectedCharacter(FString UserSessionGUID, FString SelectedCharacterName);

	void OnUserSessionSetSelectedCharacterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyUserSessionSetSelectedCharacter();
	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorUserSessionSetSelectedCharacter(const FString &ErrorMsg);

	//Get Character Custom Data
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void GetCosmeticCustomCharacterData(FString UserSessionGUID, FString CharacterName);

	void OnGetCosmeticCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stats")
		void NotifyGetCosmeticCustomCharacterData(const TArray<FCustomCharacterDataStruct> &CustomCharacterData);
	UFUNCTION(BlueprintImplementableEvent, Category = "Stats")
		void ErrorGetCosmeticCustomCharacterData(const FString &ErrorMsg);

	//Add Character Custom Data
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void AddOrUpdateCosmeticCustomCharacterData(FString UserSessionGUID, FString CharacterName, FString CustomFieldName, FString CustomValue);

	void OnAddOrUpdateCosmeticCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Join Queue
	UFUNCTION(BlueprintCallable, Category = "Queue")
		void JoinQueue(FString UserSessionGUID, FString CharacterName, FString QueueName);

	void OnJoinQueueResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void NotifyJoinQueue(const int SuccessCode);
	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void ErrorJoinQueue(const FString &ErrorMsg);

	//Leave Queue
	UFUNCTION(BlueprintCallable, Category = "Queue")
		void LeaveQueue(FString UserSessionGUID, FString CharacterName, FString QueueName);

	void OnLeaveQueueResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void NotifyLeaveQueue(const int SuccessCode);
	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void ErrorLeaveQueue(const FString &ErrorMsg);

	//Get Player Groups Character Is In
	UFUNCTION(BlueprintCallable, Category = "Player Groups")
		void GetPlayerGroupsCharacterIsIn(FString UserSessionGUID, FString CharacterName, int32 PlayerGroupTypeID);

	void OnGetPlayerGroupsCharacterIsInResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Groups")
		void NotifyGetPlayerGroupsCharacterIsIn(const TArray<FPlayerGroup> &PlayerGroups);
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Groups")
		void ErrorGetPlayerGroupsCharacterIsIn(const FString &ErrorMsg);

	//Get Players in Group
	/*UFUNCTION(BlueprintCallable, Category = "Groups")
		void GetPlayersInGroup(FString PlayerGroupName);

	void OnGetPlayersInGroupResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Groups")
		void NotifyGetPlayersInGroup(const TArray<FUserCharacter> &Players);
	UFUNCTION(BlueprintImplementableEvent, Category = "Groups")
		void ErrorGetPlayersInGroup(const FString &ErrorMsg);*/

	//Leave All MAtches
	UFUNCTION(BlueprintCallable, Category = "Queue")
		void LeaveAllMatches(FString CharacterName);

	void OnLeaveAllMatchesResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void NotifyLeaveAllMatches();
	UFUNCTION(BlueprintImplementableEvent, Category = "Queue")
		void ErrorLeaveAllMatches(const FString &ErrorMsg);


	//Get Map Server to Travel To
	UFUNCTION(BlueprintCallable, Category = "Travel")
		void GetMapServerToTravelTo(FString ZoneName);

	void OnGetMapServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Travel")
		void NotifyMapServerToTravelTo(const FString &ServerAndPort);
	UFUNCTION(BlueprintImplementableEvent, Category = "Travel")
		void ErrorMapServerToTravelTo(const FString &ErrorMsg);

	//RemoveCharacter
	UFUNCTION(BlueprintCallable, Category = "Player")
		void RemoveCharacter(FString UserSessionGUID, FString CharacterName);

	void OnRemoveCharacterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void NotifyRemoveCharacter();
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void ErrorRemoveCharacter(const FString &ErrorMsg);
};
