// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EtherlinkerSettings.generated.h"

/**
* Implements the settings for the Etherlinker plugin.
*/
UCLASS(config = Engine, defaultconfig)
class ETHERLINKER_API UEtherlinkerSettings : public UObject
{
	GENERATED_UCLASS_BODY()
			
	// Directory with .sol contracts to compile
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString ContractsToCompileDirectory;

	// Directory with integration server sources
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString IntegrationServerSourceDirectory;

	// Integration server URL to connect
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString IntegrationServerURL;

	// Integration server parameters
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString IntegrationServerParameters;

	// Is integration server running on local machine
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	bool IsLocalIntegrationServer;

	// Is integration server running on local machine
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	bool RunIntegrationServerSilently;

	// Name of script to compile contracts, located in Content/IntegrationServer
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString CompileContractsScript;

	// Name of script to compile integration server, located in Content/IntegrationServer
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString CompileIntegrationServerScript;

	// Default wallet path (where Integration Server will place wallet files)
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString DefaultWalletPath;

	// Infura URL
	UPROPERTY(config, EditAnywhere, Category = "Etherlinker Settings")
	FString InfuraURL;

};
