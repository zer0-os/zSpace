// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSGamePlayerController/ZSGamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "OWSPlayerState.h"
#include "Blueprint/UserWidget.h"

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
