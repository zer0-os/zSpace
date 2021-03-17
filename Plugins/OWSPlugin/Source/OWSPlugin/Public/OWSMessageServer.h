// Copyright 2017 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Messaging/Public/IMessageBus.h"
#include "Runtime/Messaging/Public/IMessageContext.h"
#include "Runtime/MessagingCommon/Public/MessageEndpoint.h"
#include "Runtime/MessagingCommon/Public/MessageEndpointBuilder.h"
#include "GameFramework/Actor.h"
#include "OWSMessageServer.generated.h"


USTRUCT()
struct FPlayerLocationMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Player")
		FString PlayerName;
	UPROPERTY(EditAnywhere, Category = "Player")
		FString ZoneNamesToNotify;
	UPROPERTY(EditAnywhere, Category = "Player")
		int CharacterModel;
	UPROPERTY(EditAnywhere, Category = "Player")
		FString AdditionalCharacterSettings;
	UPROPERTY(EditAnywhere, Category = "Player")
		float X;
	UPROPERTY(EditAnywhere, Category = "Player")
		float Y;
	UPROPERTY(EditAnywhere, Category = "Player")
		float Z;

	// default constructor for the receiver
	FPlayerLocationMessage() = default;
	// helper constructor for the sender
	FPlayerLocationMessage(const FString& inPlayerName, const FString& inZoneNamesToNotify, const int inCharacterModel, const FString& inAdditionalCharacterSettings, const float inX, const float inY, const float inZ)
		: PlayerName(inPlayerName) {
		ZoneNamesToNotify = inZoneNamesToNotify;
		AdditionalCharacterSettings = inAdditionalCharacterSettings;
		CharacterModel = inCharacterModel;
		X = inX;
		Y = inY;
		Z = inZ;
	}
};


UCLASS()
class OWSPLUGIN_API AOWSMessageServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOWSMessageServer();

	// initialize the Endpoint
	void Init();
	// destroy the Endpoint
	void Shutdown();
	// handle FTestMessage
	void HandlePlayerLocationMessage(const FPlayerLocationMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void OnReceivePlayerLocation(const FString &PlayerName, const FString &ZoneNamesToNotify, const int CharacterModel, const FString &AdditionalCharacterSettings, const float &X, const float &Y, const float &Z);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> TheEndpoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};



