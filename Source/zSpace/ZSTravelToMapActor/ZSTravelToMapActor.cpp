// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSTravelToMapActor/ZSTravelToMapActor.h"

#include "OWSCharacter.h"
#include "OWSPlayerController.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AZSTravelToMapActor::ComponentEndOverlap);
	
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
	Character = Cast<AOWSCharacter>(NewOtherActor);
}

void AZSTravelToMapActor::GetPlayerController(AOWSCharacter* NewOWSCharacter)
{
	if(IsValid(Character))
	{
		PlayerController = Character->GetController<AOWSPlayerController>();
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

	if(HasAuthority())
	{
		GetOWSCharacter(OtherActor);
		GetPlayerController(Character);
		GetPlayerState(PlayerController);
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
				OnShowLoadingDialogEvent.Broadcast(L_CharacterName);
				DisableCharacterMovement();
				if(IsValid(PlayerController))
				{
					GetMapServerToTravelTo(PlayerController, ERPGSchemeToChooseMap::MapWithFewestPlayers, 0);
				}
			}
		}
	}
	
}

void AZSTravelToMapActor::ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		AOWSCharacter * L_Character = Cast<AOWSCharacter>(OtherActor);
		if(IsValid(L_Character))
		{
			FTimerHandle L_TimeHandler;
			GetWorld()->GetTimerManager().SetTimer(L_TimeHandler, this, &AZSTravelToMapActor::ResetState , 5, false);
		}
	}
}

void AZSTravelToMapActor::EventErrorMapServerToTravelTo(const FString& ErrorMsg)
{
	FString Value = FString::Printf(TEXT("Ava: %s"), *ErrorMsg);
	UKismetSystemLibrary::PrintString(this, Value);
	if(IsValid(Character))
	{
		UCharacterMovementComponent * L_CharacterMovementComponent = Character->GetCharacterMovement();
		if(L_CharacterMovementComponent)
		{
			L_CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			const FString L_CharacterName = GetCharacterName();
			HideLoadingEvent(L_CharacterName);
			OnHideLoadingDialogEvent.Broadcast(L_CharacterName);
			Character->IsTransferringBetweenMaps = true;
		}
	}
}


void AZSTravelToMapActor::EventNotifyMapServerToTravelTo(const FString& ServerAndPort)
{
	FString Value = FString::Printf(TEXT("Ava: %s"), *ServerAndPort);
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
		Character = nullptr;
		PlayerController = nullptr;
		PlayerState = nullptr;
	}
}
