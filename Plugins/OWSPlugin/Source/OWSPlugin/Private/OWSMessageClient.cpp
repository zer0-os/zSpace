// Copyright 2017 Sabre Dart Studios

#include "OWSMessageClient.h"

// Sets default values
AOWSMessageClient::AOWSMessageClient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AOWSMessageClient::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerLocationEndpoint = FMessageEndpoint::Builder("PlayerLocationClientEndpoint").Build();
}

void AOWSMessageClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PlayerLocationEndpoint.Reset();

	Super::EndPlay(EndPlayReason);
}

void AOWSMessageClient::SendPlayerLocation(FString PlayerName, FString ZoneNamesToNotify, int CharacterModel, FString AdditionalCharacterSettings, float X, float Y, float Z)
{
	if (PlayerLocationEndpoint.IsValid())
	{
		//FMessageAddress MessageAddress;
		//MessageAddress.
		if (PlayerName != "")
		{
			PlayerLocationEndpoint->Publish<FPlayerLocationMessage>(new FPlayerLocationMessage(PlayerName, ZoneNamesToNotify, CharacterModel, AdditionalCharacterSettings, X, Y, Z), EMessageScope::Network);
		}
		//PlayerLocationEndpoint->Send(new FPlayerLocationMessage(PlayerName, ZoneNamesToNotify, X, Y, Z), MessageAddress);
	}
}

// Called every frame
void AOWSMessageClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

