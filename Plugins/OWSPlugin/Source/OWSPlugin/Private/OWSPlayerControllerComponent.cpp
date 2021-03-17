// Copyright 2020 Sabre Dart Studios

#include "OWSPlayerControllerComponent.h"
#include "OWSTravelToMapActor.h"
#include "OWSGameInstance.h"


// Sets default values for this component's properties
UOWSPlayerControllerComponent::UOWSPlayerControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("OWS2APIPath"),
		OWS2APIPath,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("OWSEncryptionKey"),
		OWSEncryptionKey,
		GGameIni
	);
}


// Called when the game starts
void UOWSPlayerControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UOWSPlayerControllerComponent::GetPlayerNameAndOWSCharacter(AOWSCharacter* OWSCharacter, FString& PlayerName)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	PlayerName = PlayerController->PlayerState->GetPlayerName();
	PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

	OWSCharacter = Cast<AOWSCharacter>(PlayerController->GetPawn());
}

AOWSPlayerState* UOWSPlayerControllerComponent::GetOWSPlayerState() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	return PlayerController->GetPlayerState<AOWSPlayerState>();
}


// Called every frame
/*
void UOWSPlayerControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

void UOWSPlayerControllerComponent::TravelToMap(const FString& URL, const bool SeamlessTravel)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	UE_LOG(OWS, Warning, TEXT("TravelToMap: %s"), *URL);
	PlayerController->ClientTravel(URL, TRAVEL_Absolute, false, FGuid());
}

void UOWSPlayerControllerComponent::TravelToMap2(const FString& ServerAndPort, const float X, const float Y, const float Z, const float RX, const float RY,
	const float RZ, const FString& PlayerName, const bool SeamlessTravel)
{
	/*
	FString URL = ServerAndPort
		+ FString(TEXT("?PLX=")) + FString::SanitizeFloat(X)
		+ FString(TEXT("?PLY=")) + FString::SanitizeFloat(Y)
		+ FString(TEXT("?PLZ=")) + FString::SanitizeFloat(Z)
		+ FString(TEXT("?PRX=")) + FString::SanitizeFloat(RX)
		+ FString(TEXT("?PRY=")) + FString::SanitizeFloat(RY)
		+ FString(TEXT("?PRZ=")) + FString::SanitizeFloat(RZ)
		+ FString(TEXT("?Player=")) + FGenericPlatformHttp::UrlEncode(PlayerName);
	*/

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!GetWorld())
	{
		return;
	}

	UOWSGameInstance* GameInstance = Cast<UOWSGameInstance>(GetWorld()->GetGameInstance());

	if (!GameInstance)
	{
		return;
	}

	if (!GetOWSPlayerState())
	{
		UE_LOG(OWS, Error, TEXT("Invalid OWS Player State!  You can no longer use TravelToMap2 to connect to a server from the Select Character screen!"));

		return;
	}

	FString IDData = FString::SanitizeFloat(X)
		+ "|" + FString::SanitizeFloat(Y)
		+ "|" + FString::SanitizeFloat(Z)
		+ "|" + FString::SanitizeFloat(RX)
		+ "|" + FString::SanitizeFloat(RY)
		+ "|" + FString::SanitizeFloat(RZ)
		+ "|" + FGenericPlatformHttp::UrlEncode(GetOWSPlayerState()->GetPlayerName())
		+ "|" + GetOWSPlayerState()->UserSessionGUID;

	FString EncryptedIDData = GameInstance->EncryptWithAES(IDData, OWSEncryptionKey);

	FString URL = ServerAndPort
		+ FString(TEXT("?ID=")) + EncryptedIDData;

	UE_LOG(OWS, Warning, TEXT("TravelToMap: %s"), *URL);
	PlayerController->ClientTravel(URL, TRAVEL_Absolute, false, FGuid());
}

void UOWSPlayerControllerComponent::SetSelectedCharacterAndConnectToLastZone(FString UserSessionGUID, FString SelectedCharacterName)
{
	//Set character name and get user session

	

	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(TravelTimeout); //Set timeout

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSPlayerControllerComponent::OnSetSelectedCharacterAndConnectToLastZoneResponseReceived);
	//This is the url on which to process the request
	FString url = FString(OWS2APIPath + "api/Users/SetSelectedCharacterAndGetUserSession");

	//Trim whitespace
	SelectedCharacterName.TrimStartAndEndInline();

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(UserSessionGUID);
	FormatParams.Add(SelectedCharacterName);
	FString PostParameters = FString::Format(TEXT("{ \"UserSessionGUID\": \"{0}\", \"SelectedCharacterName\": \"{1}\" }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSPlayerControllerComponent::OnSetSelectedCharacterAndConnectToLastZoneResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(OWS, Verbose, TEXT("OnSetSelectedCharacterAndConnectToLastZone Success!"));

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			ServerTravelUserSessionGUID = JsonObject->GetStringField("UserSessionGUID");
			ServerTravelCharacterName = JsonObject->GetStringField("CharName");
			ServerTravelX = JsonObject->GetNumberField("X");
			ServerTravelY = JsonObject->GetNumberField("Y");
			ServerTravelZ = JsonObject->GetNumberField("Z");
			ServerTravelRX = JsonObject->GetNumberField("RX");
			ServerTravelRY = JsonObject->GetNumberField("RY");
			ServerTravelRZ = JsonObject->GetNumberField("RZ");

			UE_LOG(OWS, Log, TEXT("OnSetSelectedCharacterAndConnectToLastZone location is %f, %f, %f"), ServerTravelX, ServerTravelY, ServerTravelZ);

			if (ServerTravelCharacterName.IsEmpty())
			{
				//Call Error BP Event

				return;
			}

			TravelToLastZoneServer(ServerTravelCharacterName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnSetSelectedCharacterAndConnectToLastZone Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSetSelectedCharacterAndConnectToLastZone Error accessing server!"));
	}
}

void UOWSPlayerControllerComponent::TravelToLastZoneServer(FString CharacterName)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(TravelTimeout); //Set timeout

	//UE_LOG(LogTemp, Warning, TEXT("CustomerID: %s"), *RPGAPICustomerKey);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSPlayerControllerComponent::OnTravelToLastZoneServerResponseReceived);

	//CharacterName = CharacterName.Replace(TEXT(" "), TEXT("%20")); //Removed for OWS2
	//This is the url on which to process the request.
	FString url = FString(OWS2APIPath + "api/Users/GetServerToConnectTo");

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(CharacterName);
	FormatParams.Add(TEXT("GETLASTZONENAME"));
	FormatParams.Add(TEXT("0"));
	FString PostParameters = FString::Format(TEXT("{ \"CharacterName\": \"{0}\", \"ZoneName\": \"{1}\", \"PlayerGroupType\": {2} }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();

	/*
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetServerToConnectTo"));

	FString PostParameters = FString(TEXT("id=")) + CharacterName
		+ FString(TEXT("&ZoneName=GETLASTZONENAME"))
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;
	*/

	/*Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();*/
}

void UOWSPlayerControllerComponent::OnTravelToLastZoneServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ServerAndPort;

	if (!GetWorld())
	{
		return;
	}

	UOWSGameInstance* GameInstance = Cast<UOWSGameInstance>(GetWorld()->GetGameInstance());

	if (!GameInstance)
	{
		return;
	}

	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ServerIP = JsonObject->GetStringField("serverip");
			FString Port = JsonObject->GetStringField("port");

			if (ServerIP.IsEmpty() || Port.IsEmpty())
			{
				UE_LOG(OWS, Error, TEXT("OnTravelToLastZoneServerResponseReceived Cannot Get Server IP and Port!"));
				return;
			}

			ServerAndPort = ServerIP + FString(TEXT(":")) + Port.Left(4);

			UE_LOG(OWS, Warning, TEXT("OnTravelToLastZoneServerResponseReceived ServerAndPort: %s"), *ServerAndPort);

			//Encrypt data to send
			FString IDData = FString::SanitizeFloat(ServerTravelX)
				+ "|" + FString::SanitizeFloat(ServerTravelY)
				+ "|" + FString::SanitizeFloat(ServerTravelZ)
				+ "|" + FString::SanitizeFloat(ServerTravelRX)
				+ "|" + FString::SanitizeFloat(ServerTravelRY)
				+ "|" + FString::SanitizeFloat(ServerTravelRZ)
				+ "|" + FGenericPlatformHttp::UrlEncode(ServerTravelCharacterName)
				+ "|" + ServerTravelUserSessionGUID;
			FString EncryptedIDData = GameInstance->EncryptWithAES(IDData, OWSEncryptionKey);

			FString URL = ServerAndPort
				+ FString(TEXT("?ID=")) + EncryptedIDData;

			TravelToMap(URL, false);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnTravelToLastZoneServerResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnTravelToLastZoneServerResponseReceived Error accessing server!"));
	}
}

/*
void UOWSPlayerControllerComponent::GetMapServerToTravelTo(TEnumAsByte<ERPGSchemeToChooseMap::SchemeToChooseMap> SelectedSchemeToChooseMap, int32 WorldServerID)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(TravelTimeout); //Set timeout

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	FString CharacterName = PlayerController->PlayerState->GetPlayerName();
	//CharacterName = CharacterName.Replace(TEXT(" "), TEXT("%20")); //Removed for OWS2

	UE_LOG(LogTemp, Warning, TEXT("PlayerName: %s"), *CharacterName);
	UE_LOG(LogTemp, Warning, TEXT("ZoneName: %s"), *ZoneName);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSTravelToMapActor::OnGetMapServerToTravelToResponseReceived);
	//This is the url on which to process the request
	FString url = FString(OWS2APIPath + "api/Users/GetServerToConnectTo");

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(CharacterName);
	FormatParams.Add(TEXT("GETLASTZONENAME"));
	FormatParams.Add(TEXT("0"));
	FString PostParameters = FString::Format(TEXT("{ \"CharacterName\": \"{0}\", \"ZoneName\": \"{1}\", \"PlayerGroupType\": {2} }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSPlayerControllerComponent::OnGetMapServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ServerAndPort;

	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ServerIP = JsonObject->GetStringField("serverip");
			FString Port = JsonObject->GetStringField("port");

			if (ServerIP.IsEmpty() || Port.IsEmpty())
			{
				ErrorMapServerToTravelTo(TEXT("Cannot connect to server!"));
				return;
			}

			ServerAndPort = ServerIP + FString(TEXT(":")) + Port.Left(4);

			UE_LOG(LogTemp, Warning, TEXT("ServerAndPort: %s"), *ServerAndPort);

			NotifyMapServerToTravelTo(ServerAndPort);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetMapServerToTravelToResponseReceived Server returned no data!"));
			ErrorMapServerToTravelTo(TEXT("There was a problem connecting to the server.  Please try again."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetMapServerToTravelToResponseReceived Error accessing server!"));
		ErrorMapServerToTravelTo(TEXT("Unknown error connecting to server!"));
	}
}
*/

void UOWSPlayerControllerComponent::SavePlayerLocation()
{
	Http = &FHttpModule::Get();
	
	/*
	APlayerController* ParentController = Cast<APlayerController>(GetOwner());

	FString PlayerName = ParentController->PlayerState->GetPlayerName();
	PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

	AOWSCharacter* MyRPGCharacter = Cast<AOWSCharacter>(ParentController->GetPawn());
	*/

	AOWSCharacter* MyRPGCharacter = NULL;
	FString PlayerName = "";
	GetPlayerNameAndOWSCharacter(MyRPGCharacter, PlayerName);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSPlayerControllerComponent::OnSavePlayerLocationResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdatePosition"));

	FString PostParameters = FString(TEXT("id=")) + PlayerName
		+ FString(TEXT("&X=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().X)
		+ FString(TEXT("&Y=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().Y)
		+ FString(TEXT("&Z=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().Z)
		+ FString(TEXT("&RX=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Roll)
		+ FString(TEXT("&RY=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Pitch)
		+ FString(TEXT("&RZ=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Yaw)
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSPlayerControllerComponent::OnSavePlayerLocationResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(OWS, Verbose, TEXT("OnSavePlayerLocationResponseReceived Success!"));
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSavePlayerLocationResponseReceived Error accessing server!"));
	}
}

void UOWSPlayerControllerComponent::SaveAllPlayerData()
{
	Http = &FHttpModule::Get();

	AOWSCharacter* MyRPGCharacter = NULL;
	FString PlayerName = "";
	GetPlayerNameAndOWSCharacter(MyRPGCharacter, PlayerName);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSPlayerControllerComponent::OnSaveAllPlayerDataResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdatePosition"));


	FString PostParameters = FString(TEXT("id=")) + PlayerName
		+ FString(TEXT("&X=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().X)
		+ FString(TEXT("&Y=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().Y)
		+ FString(TEXT("&Z=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorLocation().Z)
		+ FString(TEXT("&RX=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Roll)
		+ FString(TEXT("&RY=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Pitch)
		+ FString(TEXT("&RZ=")) + FString::SanitizeFloat(MyRPGCharacter->GetActorRotation().Yaw)
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	//UE_LOG(LogTemp, Warning, TEXT("Sent %s"), *PostParameters);
	Request->ProcessRequest();
}

void UOWSPlayerControllerComponent::OnSaveAllPlayerDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(OWS, Verbose, TEXT("OnSaveAllPlayerDataResponseReceived Success!"));
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSaveAllPlayerDataResponseReceived Error accessing server!"));
	}
}

void UOWSPlayerControllerComponent::GetChatGroupsForPlayer()
{
	Http = &FHttpModule::Get();

	AOWSCharacter* MyRPGCharacter = NULL;
	FString PlayerName = "";
	GetPlayerNameAndOWSCharacter(MyRPGCharacter, PlayerName);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSPlayerControllerComponent::OnGetChatGroupsForPlayerResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/Chat/GetChatGroupsForPlayer"));

	FString PostParameters = FString(TEXT("id=")) + PlayerName
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSPlayerControllerComponent::OnGetChatGroupsForPlayerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(OWS, Verbose, TEXT("OnGetChatGroupsForPlayerResponseReceived Success!"));

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->GetStringField("success") == "true")
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				TArray<FChatGroup> ChatGroups;

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FChatGroup tempChatGroup;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();
					tempChatGroup.ChatGroupID = tempRow->GetIntegerField("ChatGroupID");
					tempChatGroup.ChatGroupName = tempRow->GetStringField("ChatGroupName");

					ChatGroups.Add(tempChatGroup);
				}

				OnNotifyGetChatGroupsForPlayerDelegate.ExecuteIfBound(ChatGroups);
			}
		}
		else
		{
			UE_LOG(OWS, Warning, TEXT("OnGetChatGroupsForPlayerResponseReceived:  Either there were no chat groups or the JSON failed to Deserialize!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetChatGroupsForPlayerResponseReceived Error accessing server!"));
		OnErrorGetChatGroupsForPlayerDelegate.ExecuteIfBound(TEXT("OnGetChatGroupsForPlayerResponseReceived Error accessing server!"));
	}
}