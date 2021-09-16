// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSTravelToMapActor/ZSTravelToMapActor.h"
#include "OWSCharacter.h"
#include "OWSPlayerController.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/Game/ZSGamePlayerController/ZSGamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../ZSCharacterWithAbilities/ZSCharacterWithAbilities.h"
#include "../Game/ZSpaceGameInstance.h"
#include "../Game/SoundManager/SoundManager.h"
#include "../Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AZSTravelToMapActor::AZSTravelToMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	checkf(nullptr != SceneComponentRoot, TEXT("The SceneComponent is nullptr."));
	RootComponent = SceneComponentRoot;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	checkf(nullptr != BoxComponent, TEXT("The BoxComponent is nullptr."));
	BoxComponent->SetupAttachment(SceneComponentRoot);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AZSTravelToMapActor::ComponentBeginOverlap);
	
	ShowLoadingDialog = true;
}

// Called when the game starts or when spawned
void AZSTravelToMapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZSTravelToMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZSTravelToMapActor::GetOWSCharacter(AActor * NewOtherActor)
{
	Character = Cast<AZSCharacterWithAbilities>(NewOtherActor);
}

void AZSTravelToMapActor::GetPlayerController(AOWSCharacter* NewOWSCharacter)
{
	if(IsValid(Character))
	{
		PlayerController = Character->GetController<AZSGamePlayerController>();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AZSTravelToMapActor::GetPlayerController : CharacterReference is nulltpr"));
	}
}

void AZSTravelToMapActor::GetPlayerState(AOWSPlayerController * NewOWSPlayerController)
{
	if(PlayerController)
	{
		PlayerState = PlayerController->GetOWSPlayerState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AZSTravelToMapActor::GetPlayerState : PlayerController is nulltpr"));
	}
}

bool AZSTravelToMapActor::IsAvailableTravelToMap()
{
	if(IsValid(Character) && false == Character->IsTransferringBetweenMaps)
	{
		const bool L_IsEmpty = ZoneName.IsEmpty();
		if(false == L_IsEmpty )
		{
			return true;			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Already transferring!"));
	}
	return  false;
}

void AZSTravelToMapActor::DetermineDynamicSpawnAxisAndSetAppropriateOffset()
{
	if(IsValid(Character))
	{
		const FVector L_ActorLocation = GetActorLocation();
		const FVector L_CharacterLocation = Character->GetActorLocation();
		const FVector R_Location = L_ActorLocation - L_CharacterLocation;
		ZAxisOffset = R_Location.Z;
		if(DynamicAxisType::XAxis == DynamicSpawnAxis)
		{
			AxisOffset = R_Location.X;
		}
		else if(DynamicAxisType::YAxis == DynamicSpawnAxis)
		{
			AxisOffset = R_Location.Y;
		}
	}
}

FString AZSTravelToMapActor::GetCharacterName() const
{
	if(IsValid(PlayerState))
	{
		return PlayerState->GetPlayerName();
	}
	UE_LOG(LogTemp, Error, TEXT("AZSTravelToMapActor::GetCharacterName(): The PlayerState is nullptr."));
	return FString();
}

void AZSTravelToMapActor::DisableCharacterMovement()
{
	if(IsValid(Character))
	{
		Character->Server_SetAnimationState(EAnimationState::Standing);
		UCharacterMovementComponent * L_CharacterMovementComponent = Character->GetCharacterMovement();
		if(IsValid(L_CharacterMovementComponent))
		{
			L_CharacterMovementComponent->DisableMovement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AZSTravelToMapActor::DisableCharacterMovement(): The L_CharacterMovementComponent is nullptr."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AZSTravelToMapActor::DisableCharacterMovement(): The Character is nullptr."));
	}
}

void AZSTravelToMapActor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	GetOWSCharacter(OtherActor);
	GetPlayerController(Character);
	GetPlayerState(PlayerController);
	const bool bIsTeleport = IsTeleport(OtherComp);
	if(!bIsTeleport)
	{
		return;	
	}
	UZSpaceGameInstance* GameInstance = Cast<UZSpaceGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		USoundManager* SoundManager = GameInstance->GetSoundManagerObject();
		if (SoundManager)
		{
			SoundManager->PlayPortalSoundByType(EPortalSoundType::FadeIn);
			SoundManager->SetSoundVolumeBySoundClassName(FName("Ambient"), 0);
			SoundManager->SetSoundVolumeBySoundClassName(FName("Gameplay"), 0);
		}
		
		UWidgetLoadingManagerObject* WidgetLoadingManagerObject = GameInstance->GetWidgetLoadingManagerObject();
		if (IsValid(WidgetLoadingManagerObject))
		{
			WidgetLoadingManagerObject->SetNotShowLoadingWidget(false);
		}

	}
	if(HasAuthority())
	{
		if (IsValid(Character) && IsValid(PlayerController))
		{
			DisableCharacterMovement();
			Character->DisableInput(PlayerController);
			Character->Server_SetAnimationState(EAnimationState::Standing);
		}

		FTimerHandle Timer;

		GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
		{
			const bool L_IsEmpty = IsAvailableTravelToMap();
			if(L_IsEmpty)
			{
				if(UseDynamicSpawnLocation)
				{
					DetermineDynamicSpawnAxisAndSetAppropriateOffset();		
				}
				if(ShowLoadingDialog)
				{
					const FString L_CharacterName = GetCharacterName();
					ShowLoadingEvent(L_CharacterName);
					DisableCharacterMovement();
					if(IsValid(PlayerController))
					{
						GetZSMapServerToTravelTo(PlayerController, ERPGSchemeToChooseMap::MapWithFewestPlayers, 0);
					}
				}
			}
		}, 0.5f, false);
	}
}

void AZSTravelToMapActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

//	UKismetSystemLibrary::PrintString(this, "End overlap---------------------", true, false, FLinearColor::Green, 20);

	if (HasAuthority())
	{
		AOWSCharacter* L_Character = Cast<AOWSCharacter>(OtherActor);
		if (IsValid(L_Character))
		{
			FTimerHandle L_TimeHandler;
			GetWorld()->GetTimerManager().SetTimer(L_TimeHandler, this, &AZSTravelToMapActor::ResetState, 5, false);
		}
	}
}

void AZSTravelToMapActor::ShowLoadingEvent(const FString& NewCharacter)
{
	if(PlayerController)
	{
	    checkf(nullptr != UserWidgetLoading, TEXT("The UserWidgetLoading is not set, Please Set LoadingWidget class  "));
		PlayerController->ShowLoadingWidgetByCharacterName(NewCharacter, UserWidgetLoading);
	}
}

void AZSTravelToMapActor::HideLoadingEvent(const FString& NewCharacter)
{
	if(PlayerController)
	{
		PlayerController->HideLoadingWidgetByCharacterName(NewCharacter);
	}
		
}

void AZSTravelToMapActor::EventErrorMapServerToTravelTo(const FString& ErrorMsg)
{
	const FString Value = FString::Printf(TEXT("Ava: %s"), *ErrorMsg);
	UKismetSystemLibrary::PrintString(this, Value);
	if(IsValid(Character))
	{
		UCharacterMovementComponent * L_CharacterMovementComponent = Character->GetCharacterMovement();
		if(L_CharacterMovementComponent)
		{
			L_CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			const FString L_CharacterName = GetCharacterName();
			HideLoadingEvent(L_CharacterName);
			Character->IsTransferringBetweenMaps = true;
		}
	}
}


void AZSTravelToMapActor::EventNotifyMapServerToTravelTo(const FString& ServerAndPort)
{
	const FString Value = FString::Printf(TEXT("Ava: %s"), *ServerAndPort);
	UKismetSystemLibrary::PrintString(this, Value);

	UKismetSystemLibrary::PrintString(this, TEXT("Notify Map Server to Travel To"));

	if(UseDynamicSpawnLocation)
	{
		CalculatePointToMoveTo();
	}
	else
	{
		PointToMoveTo = LocationOnMap;
	}
	if(UseLineTraceToFindFloor)
	{
		const FVector L_GroundLocation = DetectGroundLocation();
		PointToMoveTo.Z = L_GroundLocation.Z;
	}
	if(IsValid(PlayerController))
	{
		const FRotator 	R_SpawnRotator = GetCharacterSpawnRotation();
		PlayerController->SaveAllPlayerData();
		const FString L_PlayerName = GetCharacterName();
		PlayerController->TravelToMap2(ServerAndPort, PointToMoveTo.X, PointToMoveTo.Y, PointToMoveTo.Z, R_SpawnRotator.Roll, R_SpawnRotator.Pitch, R_SpawnRotator.Yaw, L_PlayerName, false);
		UKismetSystemLibrary::PrintString(this, TEXT("Test: Called Travl to Map!"));
	}
}

void AZSTravelToMapActor::CalculatePointToMoveTo()
{
	const float L_AxisOffset = true == InvertDynamicAxisOffset ? AxisOffset : 0 - AxisOffset;
	if(DynamicAxisType::XAxis == DynamicSpawnAxis)
	{
		PointToMoveTo.X = LocationOnMap.X + L_AxisOffset;
		PointToMoveTo.Y = LocationOnMap.Y;
		PointToMoveTo.Z = LocationOnMap.Z + ZAxisOffset;
		
	}
	else if(DynamicAxisType::YAxis == DynamicSpawnAxis)
	{
		PointToMoveTo.X = LocationOnMap.X;
		PointToMoveTo.Y = LocationOnMap.Y + L_AxisOffset;
		PointToMoveTo.Z = LocationOnMap.Z + ZAxisOffset;
	}
		
}

FRotator AZSTravelToMapActor::GetCharacterSpawnRotation()
{
	if(UseDynamicSpawnRotation)
	{
		const FRotator R_Rotation = DynamicSpawnRotationOffeset + StartingRotation;
		return R_Rotation; 
	}
	return StartingRotation;
}

FVector AZSTravelToMapActor::DetectGroundLocation()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > L_ObjectTypes;
	L_ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	const FVector L_Start = PointToMoveTo + FVector(0, 0, 100000.0);
	const FVector L_End  = PointToMoveTo + FVector(0, 0, -100000.0);
	TArray<AActor *> L_ActorsToIgnore;
	FHitResult R_OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects(this, L_Start, L_End, L_ObjectTypes, true, L_ActorsToIgnore, EDrawDebugTrace::ForDuration, R_OutHit, true );
	R_OutHit.Location.Z += 10;
	const FVector R_Location = R_OutHit.Location;
	return R_Location;
}

void AZSTravelToMapActor::ResetState()
{
	if(IsValid(Character))
	{
		Character->IsTransferringBetweenMaps = false;
	//	Character = nullptr;
	//	PlayerController = nullptr;
	//	PlayerState = nullptr;
	}
}

bool AZSTravelToMapActor::IsTeleport(UPrimitiveComponent * NewOtherComp)
{
	bool R_Status = false;
	if (IsValid(Character))
	{
		UCapsuleComponent * L_CapsuleComponent = Character->GetCapsuleComponent();
		if(L_CapsuleComponent == NewOtherComp)
		{
			R_Status = true;
		}
	}
	return R_Status;	
}

void AZSTravelToMapActor::GetZSMapServerToTravelTo(APlayerController* NewPlayerController, TEnumAsByte<ERPGSchemeToChooseMap::SchemeToChooseMap> NewSelectedSchemeToChooseMap, int32 NewWorldServerID)
{
	HttpZS = &FHttpModule::Get();
	HttpZS->SetHttpTimeout(TravelTimeout); //Set timeout
	const FString CharacterName = NewPlayerController->PlayerState->GetPlayerName();
	UE_LOG(LogTemp, Warning, TEXT("PlayerName: %s"), *CharacterName);
	UE_LOG(LogTemp, Warning, TEXT("ZoneName: %s"), *ZoneName);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpZS->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AZSTravelToMapActor::OnGetZSMapServerToTravelToResponseReceived);
	//This is the url on which to process the request
	const FString Url = FString(OWS2APIPath + "api/Users/GetServerToConnectTo");

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(CharacterName);
	FormatParams.Add(ZoneName);
	FormatParams.Add(TEXT("0"));
	const FString PostParameters = FString::Format(TEXT("{ \"CharacterName\": \"{0}\", \"ZoneName\": \"{1}\", \"PlayerGroupType\": {2} }"), FormatParams);

	Request->SetURL(Url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AZSTravelToMapActor::OnGetZSMapServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ServerAndPort("");
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			const FString ServerIP = JsonObject->GetStringField("serverip");		
			const FString Port = JsonObject->GetStringField("port");			

			if (ServerIP.IsEmpty() || Port.IsEmpty())
			{
				ErrorMapServerToTravelTo(TEXT("Cannot connect to server!"));
				EventErrorMapServerToTravelTo(TEXT("Cannot connect to server!"));
				return;
			}

			ServerAndPort = ServerIP + FString(TEXT(":")) + Port.Left(4);		

			UE_LOG(LogTemp, Warning, TEXT("ServerAndPort: %s"), *ServerAndPort);

			NotifyMapServerToTravelTo(ServerAndPort);
			EventNotifyMapServerToTravelTo(ServerAndPort);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetMapServerToTravelToResponseReceived Server returned no data!"));
			ErrorMapServerToTravelTo(TEXT("There was a problem connecting to the server.  Please try again."));
			EventErrorMapServerToTravelTo(TEXT("There was a problem connecting to the server.  Please try again."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetMapServerToTravelToResponseReceived Error accessing server!"));
		ErrorMapServerToTravelTo(TEXT("Unknown error connecting to server!"));
		EventErrorMapServerToTravelTo(TEXT("Unknown error connecting to server!"));
	}
}
