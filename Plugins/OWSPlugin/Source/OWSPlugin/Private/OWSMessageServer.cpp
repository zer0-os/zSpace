// Copyright 2017 Sabre Dart Studios

#include "OWSMessageServer.h"


// Sets default values
AOWSMessageServer::AOWSMessageServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AOWSMessageServer::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// connect to the message bus. this will return nullptr when something fails.
		TheEndpoint = FMessageEndpoint::Builder("PlayerLocationServerEndpoint")
			.Handling<FPlayerLocationMessage>(this, &AOWSMessageServer::HandlePlayerLocationMessage)
			.ReceivingOnThread(ENamedThreads::GameThread);

		if (TheEndpoint.IsValid())
		{
			TheEndpoint->Subscribe<FPlayerLocationMessage>();
		}
	}
}

void AOWSMessageServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TheEndpoint.Reset();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AOWSMessageServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOWSMessageServer::Init()
{
	
}

void AOWSMessageServer::Shutdown()
{
	// disconnect from the message bus and stop listening for messages.
	//TheEndpoint.Reset();
}

void AOWSMessageServer::HandlePlayerLocationMessage(const FPlayerLocationMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	//UE_LOG(LogTemp, Warning, TEXT("We just received a test message from: %s"), *Message.PlayerName);

	OnReceivePlayerLocation(Message.PlayerName, Message.ZoneNamesToNotify, Message.CharacterModel, Message.AdditionalCharacterSettings, Message.X, Message.Y, Message.Z);
}