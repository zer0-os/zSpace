// Copyright 2020 Sabre Dart Studios


// ReSharper disable All
#include "zSpace/PlayerController/ZSLoginPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/BlueprintFunctionLibrary/OWSBlueprintFunctionLibrary.h"
#include "zSpace/Game/ZSpaceGameInstance.h"

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
	if (!isLoading)
	{
		OnEscButtonPressed.Broadcast();
	}
}

void AZSLoginPlayerController::CheckCharacterCountAndAdd(int32 CheckCount, const TArray<FUserCharacter>& UserCharacters)
{
	if (UserCharacters.Num() < CheckCount)
	{	
		const int32 CreateCount = CheckCount - UserCharacters.Num();
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

FCustomCharacterDataStruct AZSLoginPlayerController::GetCustomCharacterDataMeshByName(FString Character_Name) const
{
	FString CustomFieldName = UOWSBlueprintFunctionLibrary::GetMeshFieldName(this, Character_Name);
	
	for (const auto &Data : CustomCharacterDataMeshArray)
	{
		if (Data.CustomFieldName == CustomFieldName)
		{
			return Data;
		}
	}

	return FCustomCharacterDataStruct();
}

void AZSLoginPlayerController::UpdateOrAddCustomCharacterDataMesh(const FCustomCharacterDataStruct& NewData)
{
	for (auto &Data : CustomCharacterDataMeshArray)
	{
		if (Data.CustomFieldName == NewData.CustomFieldName)
		{
			Data = NewData;
			return;
		}
	}

	CustomCharacterDataMeshArray.Add(NewData);
}

FString AZSLoginPlayerController::GetUserSessionGUID() const
{
	return UserSessionGUID;
}

void AZSLoginPlayerController::SetUserSessionGUID(FString Value)
{
	UZSpaceGameInstance * ZSpaceGameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(ZSpaceGameInstance)
	{
		ZSpaceGameInstance->UserSessionGUID = Value;
	}
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

const FString AZSLoginPlayerController::ClassName = "MaleWarrior";
