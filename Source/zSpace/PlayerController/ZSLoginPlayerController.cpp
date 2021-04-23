// Copyright 2020 Sabre Dart Studios


// ReSharper disable All
#include "zSpace/PlayerController/ZSLoginPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
		const FString ClassName = "MaleWarrior";
		
		for (int32 X(0); X < CreateCount; X++)
		{
			FString NewCharacterName = GetRandomString(9);
			CreateCharacter(UserSessionGUID, NewCharacterName, ClassName);
		}
	}
}

void AZSLoginPlayerController::OnGetAllCharactersEvent(const TArray<FUserCharacter>& UserCharacters)
{
	CheckCharacterCountAndAdd(3, UserCharacters);
}

FString AZSLoginPlayerController::GetRandomString(uint8 Length)
{
	FGuid Guid = FGuid::NewGuid();
	FString GuidString = Guid.ToString();

	FString Result;
	
	for (uint8 X(0); X <= Length; X++)
	{
		int32 RandomIndex = UKismetMathLibrary::RandomInteger(GuidString.Len() - 1);
		auto Char = GuidString.GetCharArray()[RandomIndex];
		Result.AppendChar(Char);
	}

	return Result;
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
