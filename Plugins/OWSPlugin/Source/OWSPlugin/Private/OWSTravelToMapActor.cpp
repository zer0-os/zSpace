// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSTravelToMapActor.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"


// Sets default values
AOWSTravelToMapActor::AOWSTravelToMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TravelTimeout = 60.f;

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
}

// Called when the game starts or when spawned
void AOWSTravelToMapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOWSTravelToMapActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AOWSTravelToMapActor::GetMapServerToTravelTo(APlayerController* PlayerController, TEnumAsByte<ERPGSchemeToChooseMap::SchemeToChooseMap> SelectedSchemeToChooseMap, int32 WorldServerID)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(TravelTimeout); //Set timeout

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
	FormatParams.Add(ZoneName);
	FormatParams.Add(TEXT("0"));
	FString PostParameters = FString::Format(TEXT("{ \"CharacterName\": \"{0}\", \"ZoneName\": \"{1}\", \"PlayerGroupType\": {2} }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();

	/*FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetServerToConnectTo"));

	FString PostParameters = FString(TEXT("id=")) + PlayerName
		+ FString(TEXT("&ZoneName=")) + ZoneName
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();*/
}

void AOWSTravelToMapActor::OnGetMapServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

FVector2D AOWSTravelToMapActor::GetSphericalFromCartesian(FVector CartesianVector)
{
	return CartesianVector.UnitCartesianToSpherical();
}

FVector AOWSTravelToMapActor::GetCartesianFromSpherical(FVector2D ShpericalVector)
{
	return ShpericalVector.SphericalToUnitCartesian();
}