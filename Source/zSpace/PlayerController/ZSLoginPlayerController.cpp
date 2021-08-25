// Copyright 2020 Sabre Dart Studios


// ReSharper disable All
#include "zSpace/PlayerController/ZSLoginPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/BlueprintFunctionLibrary/OWSBlueprintFunctionLibrary.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"


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
		static FCharacterArrayData L_CharacterArrayData;
		L_CharacterArrayData.Empty();
		const int32 CreateCount = CheckCount - UserCharacters.Num();
		for (int32 X(0); X < CreateCount; X++)
		{
			FString NewCharacterName = GetRandomString(9);
			const FString CustomFieldName = UOWSBlueprintFunctionLibrary::GetMeshFieldName(this, NewCharacterName);
			FString DefaultSkeletalMeshName = ""; 
			CharacterMeshes->GetDefaultSkeletalMeshName(DefaultSkeletalMeshName);
			CreateCharacter(UserSessionGUID, NewCharacterName, ClassName);
			L_CharacterArrayData.CharacterNameArray.Add(NewCharacterName);
			L_CharacterArrayData.CustomFieldNameArray.Add(CustomFieldName);
			L_CharacterArrayData.DefaultSkeletalMeshNameArray.Add(DefaultSkeletalMeshName);
		}

		PostAddOrUpdateCosmeticCustomCharacterData(L_CharacterArrayData);
	}
}

void  AZSLoginPlayerController::PostAddOrUpdateCosmeticCustomCharacterData(const FCharacterArrayData & CharacterArrayData)
{
	FTimerHandle L_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(L_TimerHandle, [&]()
	{
		for (int32 I(0); I < CharacterArrayData.CharacterNameArray.Num(); I++)
		{
			ZSAddOrUpdateCosmeticCustomCharacterData(UserSessionGUID, CharacterArrayData.CharacterNameArray[I],
														            CharacterArrayData.CustomFieldNameArray[I],
														            CharacterArrayData.DefaultSkeletalMeshNameArray[I]);
		}
	}, 1, false);	
}

void AZSLoginPlayerController::ZSAddOrUpdateCosmeticCustomCharacterData(FString UserSessionGUIDParam, FString CharacterNameParam, FString CustomFieldName, FString CustomValue)
{
	FHttpModule* L_Http = &FHttpModule::Get();
	checkf(nullptr != L_Http, TEXT("The LHttp is nullptr."));
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = L_Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AZSLoginPlayerController::ZSOnAddOrUpdateCosmeticCustomCharacterDataResponseReceived);

	CharacterNameParam = CharacterNameParam.Replace(TEXT(" "), TEXT("%20"));
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/AddOrUpdateCosmeticCharacterData"));

	FString PostParameters = FString(TEXT("id=")) + CharacterNameParam
		+ FString(TEXT("&UserSessionGUID=")) + UserSessionGUID
		+ FString(TEXT("&FieldName=")) + CustomFieldName
		+ FString(TEXT("&FieldValue=")) + CustomValue
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	UE_LOG(LogTemp, Log, TEXT("OnAddOrUpdateCosmeticCustomCharacterDataResponseReceived - PlayerName: %s, FieldName: %s, Value: %s"), *CharacterNameParam, *CustomFieldName, *CustomValue);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AZSLoginPlayerController::ZSOnAddOrUpdateCosmeticCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	static int32 SL_CallCount = 0;
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			//UE_LOG(OWS, Verbose, TEXT("OnAddOrUpdateCosmeticCustomCharacterDataResponseReceived Success!"));
		}
		else
		{
			//UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCosmeticCustomCharacterDataResponseReceived Server returned no data!"));
		}
	}
	else
	{
		//UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCosmeticCustomCharacterDataResponseReceived Error accessing server!"));
	}

	ZSNotifyAddOrUpdateCosmeticCustomCharacterData(SL_CallCount++);
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
