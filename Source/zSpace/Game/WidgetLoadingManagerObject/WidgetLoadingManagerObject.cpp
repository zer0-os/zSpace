// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "OculusFunctionLibrary.h"
#include "XRLoadingScreenFunctionLibrary.h"
#include "../ZSGamePlayerController/ZSGamePlayerController.h"
#include "zSpace/ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include <GameFramework/Controller.h>

void UWidgetLoadingManagerObject::SetNotShowLoadingWidget(bool NewNotShowLoadingWidget)
{

	
	bLastNotShowLoadingWidget = bNotShowLoadingWidget;
	bNotShowLoadingWidget = NewNotShowLoadingWidget;	

	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
	{
		bNotShowLoadingWidget = bLastNotShowLoadingWidget;
	}
	 ,2.f, false );

}

void UWidgetLoadingManagerObject::SetZSpaceGameInstance(UZSpaceGameInstance* NewZSpaceGameInstance)
{
	ZSpaceGameInstance = NewZSpaceGameInstance;	
}

void UWidgetLoadingManagerObject::ShowLoadingWidget(TSubclassOf<UUserWidget> NewLoadingWidget, int32 NewZOrder)
{
	if(bNotShowLoadingWidget)
	{
		return;
	}
	checkf(NewLoadingWidget != nullptr, TEXT("The NewLoadingWidget is nullptr."));
	const EOculusDeviceType L_OculusDeviceType = UOculusFunctionLibrary::GetDeviceType();
	if(EOculusDeviceType::OculusUnknown == L_OculusDeviceType)
	{
		if(NewLoadingWidget)
		{
			//HideLoadingWidget();
			UGameInstance * GameInstance = UGameplayStatics::GetGameInstance(GetOuter());
			UserWidgetLoading = CreateWidget<UUserWidget>(GameInstance, NewLoadingWidget);
			if(UserWidgetLoading)
			{
				UserWidgetLoading->AddToViewport(NewZOrder);
			}
		}
	}
	else if(EOculusDeviceType::Quest_Link == L_OculusDeviceType)
	{
		UXRLoadingScreenFunctionLibrary::AddLoadingScreenSplash(TextureLoading, FVector(0), FRotator(0), FVector2D(1, 1));
	}

	FTimerHandle OutTimer;

	GetWorld()->GetTimerManager().SetTimer(OutTimer, [&] ()
	{

		EnableOrDisableCharacterMovement(false);

	}, 0.2f, false);
}

void UWidgetLoadingManagerObject::HideLoadingWidget()
{
	const EOculusDeviceType L_OculusDeviceType = UOculusFunctionLibrary::GetDeviceType();
	if(EOculusDeviceType::OculusUnknown == L_OculusDeviceType)
	{
		if(IsValid(UserWidgetLoading))
		{
			// RF_BeginDestroyed
			//UserWidgetLoading->BeginDestroy();
			UserWidgetLoading->RemoveFromViewport();
			UserWidgetLoading = nullptr;
		}
	}
	else if(EOculusDeviceType::Quest_Link == L_OculusDeviceType)
	{
		UXRLoadingScreenFunctionLibrary::HideLoadingScreen();
	}
	EnableOrDisableCharacterMovement(true);
}

void UWidgetLoadingManagerObject::EnableOrDisableCharacterMovement(bool isEnable)
{
	AZSCharacterWithAbilities* PlayerCharacter = Cast<AZSCharacterWithAbilities>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter)
	{
		AZSGamePlayerController* PC = Cast<AZSGamePlayerController>(PlayerCharacter->GetController());
		if (IsValid(PC))
		{
			if (isEnable)
			{
				PlayerCharacter->EnableInput(PC);
				//UKismetSystemLibrary::PrintString(this, "Enable input++++++++++++++++++++++++++", true, false, FLinearColor::Green, 15.f);
			}
			else
			{
				PlayerCharacter->DisableInput(PC);
				//UKismetSystemLibrary::PrintString(this, "Disable input++++++++++++++++++++++++++", true, false, FLinearColor::Red, 15.f);
			}
		}
	}
}
