// Copyright 2020 Sabre Dart Studios


// ReSharper disable All
#include "zSpace/PlayerController/ZSPlayerController.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Character.h>


#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Kismet/KismetMathLibrary.h"

AZSPlayerController::AZSPlayerController()
{
	OnGetAllCharacters.AddDynamic(this, &AZSPlayerController::OnGetAllCharactersEvent);	
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

void AZSPlayerController::BindOnGetAllCharacters(const TArray<FUserCharacter>& UserCharacters)
{
	OnGetAllCharacters.Broadcast(UserCharacters);
}

void AZSPlayerController::OnEscOnClicked()
{
	OnEscButtonPressed.Broadcast();
}

void AZSPlayerController::CheckCharacterCountAndAdd(int32 CheckCount, const TArray<FUserCharacter>& UserCharacters)
{
	if (UserCharacters.Num() < CheckCount)
	{	
		const int32 CreateCount = CheckCount - UserCharacters.Num();
		FString CharacterName;
		const FString ClassName = "MaleWarrior";
		
		for (int32 X(0); X < CreateCount; X++)
		{
			int32 RandomeNumberOne = UKismetMathLibrary::RandomInteger(512);
			int32 RandomeNumberTwo = UKismetMathLibrary::RandomInteger(512);
			int32 Number = RandomeNumberOne ^ RandomeNumberTwo;
			
			CharacterName = "Generated User" + FString::FromInt(Number);
			
			CreateCharacter(UserSessionGUID, CharacterName, ClassName);
		}
	}
}

void AZSPlayerController::OnGetAllCharactersEvent(const TArray<FUserCharacter>& UserCharacters)
{
	UKismetSystemLibrary::PrintString(this, FString::FromInt(UserCharacters.Num()));
	CheckCharacterCountAndAdd(3, UserCharacters);
}

FString AZSPlayerController::GetUserSessionGUID() const
{
	return UserSessionGUID;
}

void AZSPlayerController::SetUserSessionGUID(FString Value)
{
	UserSessionGUID = Value;
}
