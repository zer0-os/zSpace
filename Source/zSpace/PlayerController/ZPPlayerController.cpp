// Copyright 2020 Sabre Dart Studios


#include "zSpace/PlayerController/ZPPlayerController.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Character.h>

AZPPlayerController::AZPPlayerController()
{
	
}

void AZPPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AZPPlayerController::Tick(float NewDeltaSeconds)
{
	Super::Tick(NewDeltaSeconds);

}

void AZPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		InputComponent->BindAction("ToPreviousMenu", IE_Pressed, this, &AZPPlayerController::OnEscOnClicked);
	}
}

void AZPPlayerController::OnEscOnClicked()
{
	OnEscButtonPressed.Broadcast();
}
