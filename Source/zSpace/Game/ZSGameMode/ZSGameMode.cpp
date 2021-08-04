// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSGameMode/ZSGameMode.h"

#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APawn* AZSGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{

	AOWSPlayerState* NewPlayerState = CastChecked<AOWSPlayerState>(NewPlayer->PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Location is %f, %f, %f"), NewPlayerState->PlayerStartLocation.X, NewPlayerState->PlayerStartLocation.Y, NewPlayerState->PlayerStartLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Rotation is %f, %f, %f"), NewPlayerState->PlayerStartRotation.Roll, NewPlayerState->PlayerStartRotation.Pitch, NewPlayerState->PlayerStartRotation.Yaw);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	FVector SpawnLocation = NewPlayerState->PlayerStartLocation;
	SpawnLocation.Z += 96;	
	SpawnInfo.bDeferConstruction = false;
	APawn *  R_Pawn  = GetWorld()->SpawnActor<APawn>(GetDefaultPawnClassForController(NewPlayer), SpawnLocation, NewPlayerState->PlayerStartRotation, SpawnInfo);

	if (R_Pawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't spawn Pawn in SpawnDefaultPawnFor_Implementation"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("************************** The Spawned actor *******************"));
	}

	return R_Pawn;
}

