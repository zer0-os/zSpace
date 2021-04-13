// Copyright 2020 Sabre Dart Studios


#include "zSpace/PlayerController/ZSPlayerController.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Character.h>

AZSPlayerController::AZSPlayerController()
{
	
}

void AZSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AZSPlayerController::Tick(float NewDeltaSeconds)
{
	Super::Tick(NewDeltaSeconds);

}

void AZSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		InputComponent->BindAction("ToPreviousMenu", IE_Pressed, this, &AZSPlayerController::OnEscOnClicked);
	}
}

void AZSPlayerController::OnEscOnClicked()
{
	OnEscButtonPressed.Broadcast();
}
