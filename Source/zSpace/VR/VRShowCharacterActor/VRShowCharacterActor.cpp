// Copyright 2020 Sabre Dart Studios


#include "VRShowCharacterActor.h"

#include "Kismet/GameplayStatics.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/PlayerController/ZSLoginPlayerController.h"

// Sets default values
AVRShowCharacterActor::AVRShowCharacterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	checkf(nullptr != RootSceneComponent, TEXT("The RootSceneComponent is nullptr."));
	RootComponent = RootSceneComponent;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	checkf(nullptr != SkeletalMeshComponent, TEXT("The SkeletalMeshComponent is nullptr."));
	SkeletalMeshComponent->SetupAttachment(RootSceneComponent);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("OWS2APIPath"),
		OWS2APIPath,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("OWSEncryptionKey"),
		OWSEncryptionKey,
		GGameIni
	);
	
	
}

// Called when the game starts or when spawned
void AVRShowCharacterActor::BeginPlay()
{
	Super::BeginPlay();
	BindGetAllCharacter();	
	
}

// Called every frame
void AVRShowCharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRShowCharacterActor::BindGetAllCharacter()
{
	AZSLoginPlayerController * PC = Cast<AZSLoginPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(IsValid(PC))
	{
		PC->OnGetAllCharacters.AddUniqueDynamic(this, &AVRShowCharacterActor::OnGetAllCharacters);
	}
}

void AVRShowCharacterActor::OnGetAllCharacters(const TArray<FUserCharacter>& NewUserCharacters)
{
	UserCharacters = NewUserCharacters;
	UE_LOG(LogTemp, Log, TEXT("The UserCharactes Num = %d"), NewUserCharacters.Num());
	AZSLoginPlayerController * PC = Cast<AZSLoginPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(NewUserCharacters.Num() > 0)
	{
		const FString L_CharacterName = NewUserCharacters[0].CharacterName;
		UZSpaceGameInstance * GameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(this));
		const FString L_UserSessionGUID = GameInstance->UserSessionGUID;
		if(IsValid(PC))
		{
			PC->SetSelectedCharacterAndConnectToLastZone(L_UserSessionGUID, L_CharacterName);
		}
		//GetCosmeticCustomCharacterData(L_UserSessionGUID, L_CharacterName);
		
	}
	
}

void AVRShowCharacterActor::GetCosmeticCustomCharacterData(FString UserSessionGUID, FString CharacterName)
{
	Http = &FHttpModule::Get();

	FString PlayerName = CharacterName;
	PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AVRShowCharacterActor::OnGetCosmeticCustomCharacterDataResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetCustomCharacterData"));

	FString PostParameters = FString(TEXT("id="))  + PlayerName
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AVRShowCharacterActor::OnGetCosmeticCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FCustomCharacterDataStruct> CustomCharacterData;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FCustomCharacterDataStruct tempCustomData;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();
					tempCustomData.CustomFieldName = tempRow->GetStringField("CustomFieldName").Replace(TEXT("COSMETIC_"), TEXT(""));
					tempCustomData.FieldValue = tempRow->GetStringField("FieldValue");

					CustomCharacterData.Add(tempCustomData);
				}
			}

			//NotifyGetCosmeticCustomCharacterData(CustomCharacterData);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatusesResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatusesResponseReceived Error accessing server!"));
	}
	
}

