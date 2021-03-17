// Copyright 2017 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Messaging/Public/IMessageBus.h"
#include "Runtime/Messaging/Public/IMessageContext.h"
#include "Runtime/MessagingCommon/Public/MessageEndpoint.h"
#include "Runtime/MessagingCommon/Public/MessageEndpointBuilder.h"
#include "OWSMessageServer.h"
#include "GameFramework/Actor.h"
#include "OWSMessageClient.generated.h"

UCLASS()
class OWSPLUGIN_API AOWSMessageClient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOWSMessageClient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SendPlayerLocation(FString PlayerName, FString ZoneNamesToNotify, int CharacterModel, FString AdditionalCharacterSettings, float X, float Y, float Z);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> PlayerLocationEndpoint;
	
};
