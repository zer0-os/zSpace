// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSEtherlinkerGameMode/ZSEtherlinkerGameMode.h"

#include "EtherlinkerFunctionLibrary.h"

AZSEtherlinkerGameMode::AZSEtherlinkerGameMode()
{
		
}

void AZSEtherlinkerGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartIntegrationServer();	
}

void AZSEtherlinkerGameMode::StartIntegrationServer()
{
	const bool L_IsStatus = UEtherlinkerFunctionLibrary::CheckIntegrationServer();
	if(false == L_IsStatus)
	{
		const bool L_IsRunning = UEtherlinkerFunctionLibrary::StartIntegrationServer();
		if(false == L_IsRunning)
		{
			UE_LOG(LogTemp, Error, TEXT("The Integratino server is not started."));	
		}
	}
}
