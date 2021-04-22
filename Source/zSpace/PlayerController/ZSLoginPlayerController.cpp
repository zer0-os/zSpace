// Copyright 2020 Sabre Dart Studios


// ReSharper disable All
#include "zSpace/PlayerController/ZSLoginPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

AZSLoginPlayerController::AZSLoginPlayerController()
{
	OnGetAllCharacters.AddDynamic(this, &AZSLoginPlayerController::OnGetAllCharactersEvent);	
}

void AZSLoginPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AZSLoginPlayerController::Tick(float NewDeltaSeconds)
{
	Super::Tick(NewDeltaSeconds);
}

void AZSLoginPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		InputComponent->BindAction("ToPreviousMenu", IE_Pressed, this, &AZSLoginPlayerController::OnEscOnClicked);
	}
}

void AZSLoginPlayerController::BindOnGetAllCharacters(const TArray<FUserCharacter>& UserCharacters)
{
	OnGetAllCharacters.Broadcast(UserCharacters);
}

void AZSLoginPlayerController::OnEscOnClicked()
{
	OnEscButtonPressed.Broadcast();
}

void AZSLoginPlayerController::CheckCharacterCountAndAdd(int32 CheckCount, const TArray<FUserCharacter>& UserCharacters)
{
	if (UserCharacters.Num() < CheckCount)
	{	
		const int32 CreateCount = CheckCount - UserCharacters.Num();
		FString NewCharacterName;
		const FString ClassName = "MaleWarrior";
		
		for (int32 X(0); X < CreateCount; X++)
		{
			int32 RandomeNumberOne = UKismetMathLibrary::RandomInteger(512);
			int32 RandomeNumberTwo = UKismetMathLibrary::RandomInteger(512);
			int32 Number = RandomeNumberOne ^ RandomeNumberTwo;
			
			NewCharacterName = "Generated User" + FString::FromInt(Number);
			
			CreateCharacter(UserSessionGUID, CharacterName, ClassName);
		}
	}
}

void AZSLoginPlayerController::OnGetAllCharactersEvent(const TArray<FUserCharacter>& UserCharacters)
{
	CheckCharacterCountAndAdd(3, UserCharacters);
}

FString AZSLoginPlayerController::GetUserSessionGUID() const
{
	return UserSessionGUID;
}

void AZSLoginPlayerController::SetUserSessionGUID(FString Value)
{
	UserSessionGUID = Value;
}

FString AZSLoginPlayerController::GetCharacterName() const
{
	return CharacterName;
}

void AZSLoginPlayerController::SetCharacterName(FString Value)
{
	CharacterName = Value;
}
