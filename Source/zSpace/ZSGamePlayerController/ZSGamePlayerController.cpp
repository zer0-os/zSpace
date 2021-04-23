// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSGamePlayerController/ZSGamePlayerController.h"

#include "OWSCharacterWithAbilities.h"
#include "OWSGameMode.h"
#include "Net/UnrealNetwork.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "OWSPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AZSGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AZSGamePlayerController, member);

}

AZSGamePlayerController::AZSGamePlayerController()
{
		
}

bool AZSGamePlayerController::CheckCharacterNameOwnedPlayerController(const FString& NewCharacterName)
{
	AOWSPlayerState * L_PlayerState = GetPlayerState<AOWSPlayerState>();
	if(IsValid(L_PlayerState) && NewCharacterName.Equals(L_PlayerState->GetPlayerName()))
	{
		return true;	
	}
	return false;
}


void AZSGamePlayerController::ShowLoadingWidgetByCharacterName(const FString& NewCharacterName, TSubclassOf<class UUserWidget> NewUserWidgetSubClass)
{
	// [Server]
	if(ROLE_Authority == GetLocalRole())
	{
		const bool L_Status = CheckCharacterNameOwnedPlayerController(NewCharacterName);
		if(L_Status)
		{
			Client_ShowLoadingWidgetByWidget(NewUserWidgetSubClass);
		}
	}
}

void AZSGamePlayerController::HideLoadingWidgetByCharacterName(const FString& NewCharacterName)
{
	// [Server]
	if(ROLE_Authority == GetLocalRole())
	{
		const bool L_Status = CheckCharacterNameOwnedPlayerController(NewCharacterName);
		if(L_Status)
		{
			Client_HideLoadingWidget();		
		}
	}
	
}


void AZSGamePlayerController::Client_HideLoadingWidget_Implementation()
{
	UZSpaceGameInstance * GameInstance = Cast<UZSpaceGameInstance>(GetGameInstance());
	checkf( nullptr != GameInstance, TEXT("The GameInstance is not casted to UZSpaceGameInstance."));
	if(IsValid(GameInstance))
	{
		UWidgetLoadingManagerObject * L_WidgetLoadingManagerObject = GameInstance->GetWidgetLoadingManagerObject();
		checkf(nullptr != L_WidgetLoadingManagerObject, TEXT("The L_WidgetLocadingManagerObjecct is nullptr."));
		if(L_WidgetLoadingManagerObject)
		{
			L_WidgetLoadingManagerObject->HideLoadingWidget();
		}
	}
}

void AZSGamePlayerController::Client_ShowLoadingWidgetByWidget_Implementation( TSubclassOf<UUserWidget> NewUserWidgetSubClass)
{
	UZSpaceGameInstance * GameInstance = Cast<UZSpaceGameInstance>(GetGameInstance());
	checkf( nullptr != GameInstance, TEXT("The GameInstance is not casted to UZSpaceGameInstance."));
	if(IsValid(GameInstance))
	{
		UWidgetLoadingManagerObject * L_WidgetLoadingManagerObject = GameInstance->GetWidgetLoadingManagerObject();
		checkf(nullptr != L_WidgetLoadingManagerObject, TEXT("The L_WidgetLocadingManagerObjecct is nullptr."));
		if(L_WidgetLoadingManagerObject)
		{
			checkf(nullptr != NewUserWidgetSubClass, TEXT("The NewUserWidgetSubClass is not set. Playes set NewUserWidgetSubClass "));
			if(NewUserWidgetSubClass)
			{
				L_WidgetLoadingManagerObject->ShowLoadingWidget(NewUserWidgetSubClass,99);
			}
		}
	}
}

FVector  AZSGamePlayerController::GetClosePlayerStart()
{
	FVector R_Location = FVector::ZeroVector;
	R_Location.Z += 380;
	TArray<AActor *> OutActor;
	const APawn * L_Pawn = GetPawn<APawn>();
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActor);
	const FVector L_PawnPlayerLocation = L_Pawn->GetActorLocation();	
	OutActor.Sort([L_PawnPlayerLocation] (const AActor & A,  const AActor & B)
	{
		const float DistanceA = FVector::DistSquared(L_PawnPlayerLocation, A.GetActorLocation());
		const float DistanceB = FVector::DistSquared(L_PawnPlayerLocation, B.GetActorLocation());
		return DistanceA > DistanceB;
	});
	if(OutActor.IsValidIndex(0))
	{
		R_Location = OutActor[0]->GetActorLocation();
		R_Location.Z += 380;
	}
	UE_LOG(LogTemp, Warning, TEXT("ZSpace Location = %s"),  *R_Location.ToString());
	return R_Location;
}

bool AZSGamePlayerController::ReTeleport_Validate()
{
	return true;
}

void AZSGamePlayerController::ReTeleport_Implementation()
{
	if(ROLE_Authority == GetLocalRole())
	{
		const FVector L_Location = GetClosePlayerStart();
		AOWSGameMode *  GameMode =  GetGameMode();	
		checkf(nullptr != GameMode, TEXT("The GameMode is nullptr."));
		if(IsValid(GameMode))
		{
			const FString L_ZoneName = GameMode->IAmZoneName;
			GetMapServerToTravelTo(L_ZoneName);
			AOWSCharacterWithAbilities * L_Player = GetPawn<AOWSCharacterWithAbilities>();
			if(IsValid(L_Player))
			{
				L_Player->IsTransferringBetweenMaps = true;	
			}
		}
	}
	
}


