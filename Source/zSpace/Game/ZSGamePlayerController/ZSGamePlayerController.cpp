// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSGamePlayerController/ZSGamePlayerController.h"
#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "zSpace/Components/ManageWidgetsResolution.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "OWSCharacterWithAbilities.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "OWSPlayerState.h"
#include "OWSGameMode.h"
#include "Components/ZSEthereumActorComponent/ZSEthereumActorComponent.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherlinkerRemoteWalletManager/ZSEtherlinkerRemoteWalletManager.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherManager/ZSEtherManager.h"


AZSGamePlayerController::AZSGamePlayerController()
{
			
	ZSEtherManager = CreateDefaultSubobject<UZSEtherManager>(TEXT("ZSEtherManager"));
	checkf(nullptr != ZSEtherManager, TEXT("The ZSEtherManager is nullptr."));
	AddOwnedComponent(ZSEtherManager);
	
	ZSEtherlinkerRemoteWalletManager  = CreateDefaultSubobject<UZSEtherlinkerRemoteWalletManager>(TEXT("ZSEtherlinkerRemoteWalletManager"));
	checkf(nullptr != ZSEtherlinkerRemoteWalletManager, TEXT("The ZSEtherlinkerRemoteWalletManager is nullptr.") );
	AddOwnedComponent(ZSEtherlinkerRemoteWalletManager);

	ZSEthereumActorComponent = CreateDefaultSubobject<UZSEthereumActorComponent>(TEXT("ZSEthereumActorComponent"));
	checkf(nullptr != ZSEthereumActorComponent, TEXT("The ZSEthereumActorComponent is nullptr."));
	AddOwnedComponent(ZSEthereumActorComponent);
	
}

void AZSGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AZSGamePlayerController, member);
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

void AZSGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (!IsValid(InputComponent)) return;

	InputComponent->BindAction(FName("HideOrShowGameplayWidget"), IE_Pressed, this, &AZSGamePlayerController::HideOrShowGameplayWidget);
}

void AZSGamePlayerController::HideOrShowGameplayWidget()
{
	UUserWidget* Widget = UUIBlueprintFunctionLibrary::GetWidgetByWidgetType(this, EWidgetType::Gameplay);
	if (IsValid(Widget))
	{
		UZSpaceGameInstance* GameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (!IsValid(GameInstance)) return;

		UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
		if (!IsValid(ManageWidgetsResolution)) return;
		
		if (Widget->IsInViewport())
		{
			ManageWidgetsResolution->SetIsGameplayWidgetHidden(true);
			Widget->RemoveFromParent();
		}
		else
		{
			ManageWidgetsResolution->SetIsGameplayWidgetHidden(false);
			Widget->AddToViewport();
		}
	}
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


UZSEthereumActorComponent* AZSGamePlayerController::GetZSEthereumActorComponent_Implementation()
{
	return ZSEthereumActorComponent;
}

UZSEtherManager* AZSGamePlayerController::GetZSEtherManager_Implementation()
{
	return ZSEtherManager;
}

UZSEtherlinkerRemoteWalletManager* AZSGamePlayerController::GetZSEtherlinkerRemoteWalletManager_Implementation()
{
	return ZSEtherlinkerRemoteWalletManager;	
}

void AZSGamePlayerController::UseTerminal(UEthereumTerminalComponent* NewEthereumTerminalComponent)
{
	if(IsValid(NewEthereumTerminalComponent))
	{
		if(GetRemoteRole() < ROLE_Authority)
		{
			Server_UserTerminal(NewEthereumTerminalComponent);
			return;		
		}
		APawn * L_Pawn = GetPawn();
		checkf(nullptr != L_Pawn, TEXT("The L_Pawn is nullptr."));
		if(IsValid(L_Pawn))
		{
			NewEthereumTerminalComponent->Use(L_Pawn);
		}
	}
		
}

bool AZSGamePlayerController::Server_UserTerminal_Validate(UEthereumTerminalComponent* NewEthereumTerminalComponent)
{
	if(IsValid(NewEthereumTerminalComponent))
	{
		return false;
	}
	return true;
}

void AZSGamePlayerController::Server_UserTerminal_Implementation(UEthereumTerminalComponent* NewEthereumTerminalComponent)
{
	UseTerminal(NewEthereumTerminalComponent);	
}

