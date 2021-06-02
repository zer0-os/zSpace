// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSCharacter.h"
#include "OWSPlugin.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Core/Public/Misc/Guid.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"
#include "OWSPlayerController.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

// Sets default values
AOWSCharacter::AOWSCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsTransferringBetweenMaps = false;

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

	Http = &FHttpModule::Get();

	TeamId = FGenericTeamId(TeamNumber);

	bShouldAutoLoadCustomCharacterStats = false;

	//AlwaysRelevantPartyID = 0;
}

// Called when the game starts or when spawned
void AOWSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOWSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (!IsAMob)
		{
			LoadCharacterStats();
		}
	}
}

// Called every frame
void AOWSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool AOWSCharacter::CanJumpInternal_Implementation() const
{
	const bool bCanHoldToJumpHigher = (GetJumpMaxHoldTime() > 0.0f) && IsJumpProvidingForce();

	bool CanJumpFromClimbing = Cast<UOWSCharacterMovementComponent>(GetCharacterMovement())->bIsClimbing;

	return !bIsCrouched && GetCharacterMovement() && (GetCharacterMovement()->IsMovingOnGround() || bCanHoldToJumpHigher || CanJumpFromClimbing) && GetCharacterMovement()->IsJumpAllowed() && !GetCharacterMovement()->bWantsToCrouch;
}

bool AOWSCharacter::IsCharacterFacingActor(AActor* ActorToTest, float MaxAngle)
{
	FVector VectorToActor = ActorToTest->GetActorLocation() - GetActorLocation();

	float dotProduct = FVector::DotProduct(GetActorForwardVector(), VectorToActor);

	if (dotProduct < MaxAngle)
	{
		return true;
	}

	return false;
}

bool AOWSCharacter::IsCharacterFacingActorForward(AActor* ActorToTest, float MaxAngle)
{
	float dotProduct = FVector::DotProduct(GetActorForwardVector(), ActorToTest->GetActorForwardVector());

	if (dotProduct >= MaxAngle)
	{
		return true;
	}

	return false;
}

// Called to bind functionality to input
void AOWSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AOWSPlayerController* AOWSCharacter::GetOWSPlayerController()
{
	return Cast<AOWSPlayerController>(GetController());
}

FGenericTeamId AOWSCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void AOWSCharacter::LoadCharacterStats()
{
	UE_LOG(OWS, Verbose, TEXT("AOWSCharacter: LoadCharacterStats"));
	GetCharacterStatsBase();
	if (bShouldAutoLoadCustomCharacterStats)
	{
		AutoLoadCustomCharacterStats();
	}
}

void AOWSCharacter::AutoLoadCustomCharacterStats()
{
	UE_LOG(OWS, Verbose, TEXT("AOWSCharacter: AutoLoadCustomCharacterStats"));
}

void AOWSCharacter::GetCharacterStatsBase()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();		
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetCharacterStatsBaseResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/api/RPGCharacter/GetByCharName"));

		FString PostParameters = TEXT("{ \"id\": \"") + PlayerName
			+ FString(TEXT("\", \"CustomerGUID\": \"")) + RPGAPICustomerKey + TEXT("\"");

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetCharacterStatsBaseResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			LoadCharacterStatsFromJSON(JsonObject);

			LoadCustomCharacterStats();
			UpdateCharacterStatsAfterLoading();
			OnRPGInitalizationComplete();
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetCharacterStatsResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetCharacterStatsResponseReceived Error accessing server!"));
	}
}

void AOWSCharacter::LoadCustomCharacterStats_Implementation() {

}

void AOWSCharacter::UpdateCharacterStatsAfterLoading_Implementation() {

}

void AOWSCharacter::OnRPGInitalizationComplete_Implementation() {

}



void AOWSCharacter::LoadCharacterStatsFromJSON(TSharedPtr<FJsonObject>JsonObject)
{
	UE_LOG(OWS, Verbose, TEXT("AOWSCharacter: LoadCharacterStatsFromJSON"));

	CharacterName = JsonObject->GetStringField("CharacterName");

	IsAdmin = JsonObject->GetBoolField("IsAdmin");
	IsModerator = JsonObject->GetBoolField("IsModerator");

	ClassName = JsonObject->GetStringField("ClassName");
	Gender = JsonObject->GetNumberField("Gender");
	CharacterLevel = JsonObject->GetNumberField("Level");
	XP = JsonObject->GetNumberField("XP");
	HitDice = JsonObject->GetNumberField("HitDie");
	MaxHP = JsonObject->GetNumberField("MaxHealth");
	Wounds = JsonObject->GetNumberField("Wounds");
	Strength = JsonObject->GetNumberField("Strength");
	Dexterity = JsonObject->GetNumberField("Dexterity");
	Constitution = JsonObject->GetNumberField("Constitution");
	Intellect = JsonObject->GetNumberField("Intellect");
	Wisdom = JsonObject->GetNumberField("Wisdom");
	Charisma = JsonObject->GetNumberField("Charisma");
	Spirit = JsonObject->GetNumberField("Spirit");
	Magic = JsonObject->GetNumberField("Magic");
	Fortitude = JsonObject->GetNumberField("Fortitude");
	Reflex = JsonObject->GetNumberField("Reflex");
	Willpower = JsonObject->GetNumberField("Willpower");
	BaseAttackBonus = JsonObject->GetNumberField("BaseAttackBonus");
	Speed = JsonObject->GetNumberField("Speed");
	Initiative = JsonObject->GetNumberField("Initiative");
	NaturalArmor = JsonObject->GetNumberField("NaturalArmor");
	Resistance = JsonObject->GetNumberField("Resistance");
	TeamNumber = JsonObject->GetNumberField("TeamNumber");

	//Change TeamId for AI Perception
	TeamId = FGenericTeamId(TeamNumber);

	Thirst = JsonObject->GetNumberField("Thirst");
	Hunger = JsonObject->GetNumberField("Hunger");
	Score = JsonObject->GetNumberField("Score");
	MaxHealth = JsonObject->GetNumberField("MaxHealth");
	Health = JsonObject->GetNumberField("Health");
	HealthRegenRate = JsonObject->GetNumberField("HealthRegenRate");
	MaxMana = JsonObject->GetNumberField("MaxMana");
	Mana = JsonObject->GetNumberField("Mana");
	ManaRegenRate = JsonObject->GetNumberField("ManaRegenRate");
	MaxEnergy = JsonObject->GetNumberField("MaxEnergy");
	Energy = JsonObject->GetNumberField("Energy");
	EnergyRegenRate = JsonObject->GetNumberField("EnergyRegenRate");
	MaxFatigue = JsonObject->GetNumberField("MaxFatigue");
	Fatigue = JsonObject->GetNumberField("Fatigue");
	FatigueRegenRate = JsonObject->GetNumberField("FatigueRegenRate");
	MaxStamina = JsonObject->GetNumberField("MaxStamina");
	Stamina = JsonObject->GetNumberField("Stamina");
	StaminaRegenRate = JsonObject->GetNumberField("StaminaRegenRate");
	MaxEndurance = JsonObject->GetNumberField("MaxEndurance");
	Endurance = JsonObject->GetNumberField("Endurance");
	EnduranceRegenRate = JsonObject->GetNumberField("EnduranceRegenRate");
	Agility = JsonObject->GetNumberField("Agility");
	BaseAttack = JsonObject->GetNumberField("BaseAttack");
	BaseAttackBonus = JsonObject->GetNumberField("BaseAttackBonus");
	AttackPower = JsonObject->GetNumberField("AttackPower");
	AttackSpeed = JsonObject->GetNumberField("AttackSpeed");
	CritChance = JsonObject->GetNumberField("CritChance");
	CritMultiplier = JsonObject->GetNumberField("CritMultiplier");
	Haste = JsonObject->GetNumberField("Haste");
	SpellPower = JsonObject->GetNumberField("SpellPower");
	SpellPenetration = JsonObject->GetNumberField("SpellPenetration");
	Defense = JsonObject->GetNumberField("Defense");
	Dodge = JsonObject->GetNumberField("Dodge");
	Parry = JsonObject->GetNumberField("Parry");
	Avoidance = JsonObject->GetNumberField("Avoidance");
	Versatility = JsonObject->GetNumberField("Versatility");
	Multishot = JsonObject->GetNumberField("Multishot");
	Initiative = JsonObject->GetNumberField("Initiative");
	NaturalArmor = JsonObject->GetNumberField("NaturalArmor");
	PhysicalArmor = JsonObject->GetNumberField("PhysicalArmor");
	BonusArmor = JsonObject->GetNumberField("BonusArmor");
	ForceArmor = JsonObject->GetNumberField("ForceArmor");
	MagicArmor = JsonObject->GetNumberField("MagicArmor");
	Resistance = JsonObject->GetNumberField("Resistance");
	ReloadSpeed = JsonObject->GetNumberField("ReloadSpeed");
	Range = JsonObject->GetNumberField("Range");
	Speed = JsonObject->GetNumberField("Speed");

	Gold = JsonObject->GetNumberField("Gold");
	Silver = JsonObject->GetNumberField("Silver");
	Copper = JsonObject->GetNumberField("Copper");
	FreeCurrency = JsonObject->GetNumberField("FreeCurrency");
	PremiumCurrency = JsonObject->GetNumberField("PremiumCurrency");

	Perception = JsonObject->GetNumberField("Perception");
	Acrobatics = JsonObject->GetNumberField("Acrobatics");
	Climb = JsonObject->GetNumberField("Climb");
	Stealth = JsonObject->GetNumberField("Stealth");

	Score = JsonObject->GetNumberField("Score");
}


void AOWSCharacter::UpdateCharacterStatsBase()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnUpdateCharacterStatsBaseResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdateCharacterStats"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&CharacterLevel=")) + FString::FromInt(this->CharacterLevel)
			+ FString(TEXT("&Gender=")) + FString::FromInt(this->Gender)
			+ FString(TEXT("&Weight=")) + "0"
			+ FString(TEXT("&Size=")) + "0"
			+ FString(TEXT("&Fame=")) + "0"
			+ FString(TEXT("&Alignment=")) + "0"
			+ FString(TEXT("&Description=")) + ""
			+ FString(TEXT("&XP=")) + FString::FromInt(this->XP)
			+ FString(TEXT("&X=")) + FString::SanitizeFloat(this->GetActorLocation().X)
			+ FString(TEXT("&Y=")) + FString::SanitizeFloat(this->GetActorLocation().Y)
			+ FString(TEXT("&Z=")) + FString::SanitizeFloat(this->GetActorLocation().Z)
			+ FString(TEXT("&RX=")) + FString::SanitizeFloat(this->GetActorRotation().Roll)
			+ FString(TEXT("&RY=")) + FString::SanitizeFloat(this->GetActorRotation().Pitch)
			+ FString(TEXT("&RZ=")) + FString::SanitizeFloat(this->GetActorRotation().Yaw)
			+ FString(TEXT("&TeamNumber=")) + FString::FromInt(this->TeamNumber)
			+ FString(TEXT("&HitDice=")) + FString::FromInt(this->HitDice)
			+ FString(TEXT("&Wounds=")) + FString::SanitizeFloat(this->Wounds)
			+ FString(TEXT("&Thirst=")) + FString::SanitizeFloat(this->Thirst)
			+ FString(TEXT("&Hunger=")) + FString::SanitizeFloat(this->Hunger)
			+ FString(TEXT("&MaxHealth=")) + FString::SanitizeFloat(this->MaxHealth)
			+ FString(TEXT("&Health=")) + FString::SanitizeFloat(this->Health)
			+ FString(TEXT("&HealthRegenRate=")) + FString::SanitizeFloat(this->HealthRegenRate)
			+ FString(TEXT("&MaxMana=")) + FString::SanitizeFloat(this->MaxMana)
			+ FString(TEXT("&Mana=")) + FString::SanitizeFloat(this->Mana)
			+ FString(TEXT("&ManaRegenRate=")) + FString::SanitizeFloat(this->ManaRegenRate)
			+ FString(TEXT("&MaxEnergy=")) + FString::SanitizeFloat(this->MaxEnergy)
			+ FString(TEXT("&Energy=")) + FString::SanitizeFloat(this->Energy)
			+ FString(TEXT("&EnergyRegenRate=")) + FString::SanitizeFloat(this->EnergyRegenRate)
			+ FString(TEXT("&MaxFatigue=")) + FString::SanitizeFloat(this->MaxFatigue)
			+ FString(TEXT("&Fatigue=")) + FString::SanitizeFloat(this->Fatigue)
			+ FString(TEXT("&FatigueRegenRate=")) + FString::SanitizeFloat(this->FatigueRegenRate)
			+ FString(TEXT("&MaxStamina=")) + FString::SanitizeFloat(this->MaxStamina)
			+ FString(TEXT("&Stamina=")) + FString::SanitizeFloat(this->Stamina)
			+ FString(TEXT("&StaminaRegenRate=")) + FString::SanitizeFloat(this->StaminaRegenRate)
			+ FString(TEXT("&MaxEndurance=")) + FString::SanitizeFloat(this->MaxEndurance)
			+ FString(TEXT("&Endurance=")) + FString::SanitizeFloat(this->Endurance)
			+ FString(TEXT("&EnduranceRegenRate=")) + FString::SanitizeFloat(this->EnduranceRegenRate)
			+ FString(TEXT("&Strength=")) + FString::SanitizeFloat(this->Strength)
			+ FString(TEXT("&Dexterity=")) + FString::SanitizeFloat(this->Dexterity)
			+ FString(TEXT("&Constitution=")) + FString::SanitizeFloat(this->Constitution)
			+ FString(TEXT("&Intellect=")) + FString::SanitizeFloat(this->Intellect)
			+ FString(TEXT("&Wisdom=")) + FString::SanitizeFloat(this->Wisdom)
			+ FString(TEXT("&Charisma=")) + FString::SanitizeFloat(this->Charisma)
			+ FString(TEXT("&Agility=")) + FString::SanitizeFloat(this->Agility)
			+ FString(TEXT("&Spirit=")) + FString::SanitizeFloat(this->Spirit)
			+ FString(TEXT("&Magic=")) + FString::SanitizeFloat(this->Magic)
			+ FString(TEXT("&Fortitude=")) + FString::SanitizeFloat(this->Fortitude)
			+ FString(TEXT("&Reflex=")) + FString::SanitizeFloat(this->Reflex)
			+ FString(TEXT("&Willpower=")) + FString::SanitizeFloat(this->Willpower)
			+ FString(TEXT("&BaseAttack=")) + FString::SanitizeFloat(this->BaseAttack)
			+ FString(TEXT("&BaseAttackBonus=")) + FString::SanitizeFloat(this->BaseAttackBonus)
			+ FString(TEXT("&AttackPower=")) + FString::SanitizeFloat(this->AttackPower)
			+ FString(TEXT("&AttackSpeed=")) + FString::SanitizeFloat(this->AttackSpeed)
			+ FString(TEXT("&CritChance=")) + FString::SanitizeFloat(this->CritChance)
			+ FString(TEXT("&CritMultiplier=")) + FString::SanitizeFloat(this->CritMultiplier)
			+ FString(TEXT("&Haste=")) + FString::SanitizeFloat(this->Haste)
			+ FString(TEXT("&SpellPower=")) + FString::SanitizeFloat(this->SpellPower)
			+ FString(TEXT("&SpellPenetration=")) + FString::SanitizeFloat(this->SpellPenetration)
			+ FString(TEXT("&Defense=")) + FString::SanitizeFloat(this->Defense)
			+ FString(TEXT("&Dodge=")) + FString::SanitizeFloat(this->Dodge)
			+ FString(TEXT("&Parry=")) + FString::SanitizeFloat(this->Parry)
			+ FString(TEXT("&Avoidance=")) + FString::SanitizeFloat(this->Avoidance)
			+ FString(TEXT("&Versatility=")) + FString::SanitizeFloat(this->Versatility)
			+ FString(TEXT("&Multishot=")) + FString::SanitizeFloat(this->Multishot)
			+ FString(TEXT("&Initiative=")) + FString::SanitizeFloat(this->Initiative)
			+ FString(TEXT("&NaturalArmor=")) + FString::SanitizeFloat(this->NaturalArmor)
			+ FString(TEXT("&PhysicalArmor=")) + FString::SanitizeFloat(this->PhysicalArmor)
			+ FString(TEXT("&BonusArmor=")) + FString::SanitizeFloat(this->BonusArmor)
			+ FString(TEXT("&ForceArmor=")) + FString::SanitizeFloat(this->ForceArmor)
			+ FString(TEXT("&MagicArmor=")) + FString::SanitizeFloat(this->MagicArmor)
			+ FString(TEXT("&Resistance=")) + FString::SanitizeFloat(this->Resistance)
			+ FString(TEXT("&ReloadSpeed=")) + FString::SanitizeFloat(this->ReloadSpeed)
			+ FString(TEXT("&Range=")) + FString::SanitizeFloat(this->Range)
			+ FString(TEXT("&Speed=")) + FString::SanitizeFloat(this->Speed)
			+ FString(TEXT("&Gold=")) + FString::FromInt(this->Gold)
			+ FString(TEXT("&Silver=")) + FString::FromInt(this->Silver)
			+ FString(TEXT("&Copper=")) + FString::FromInt(this->Copper)
			+ FString(TEXT("&FreeCurrency=")) + FString::FromInt(this->FreeCurrency)
			+ FString(TEXT("&PremiumCurrency=")) + FString::FromInt(this->PremiumCurrency)
			+ FString(TEXT("&Perception=")) + FString::SanitizeFloat(this->Perception)
			+ FString(TEXT("&Acrobatics=")) + FString::SanitizeFloat(this->Acrobatics)
			+ FString(TEXT("&Climb=")) + FString::SanitizeFloat(this->Climb)
			+ FString(TEXT("&Stealth=")) + FString::SanitizeFloat(this->Stealth)
			+ FString(TEXT("&Score=")) + FString::FromInt(this->Score)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnUpdateCharacterStatsBaseResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		/*if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetCharacterStatsResponseReceived Server returned no data!"));
		}*/
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetCharacterStatsResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::GetCharacterStatuses()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetCharacterStatusesResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetCharacterStatuses"));
		
		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetCharacterStatusesResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			//CharacterName = JsonObject->GetStringField("CharacterName");
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetCharacterStatusesResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetCharacterStatusesResponseReceived Error accessing server!"));
	}
}

void AOWSCharacter::NotifyGetCharacterStatuses_Implementation() {

}

void AOWSCharacter::ErrorGetCharacterStatuses_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::AddOrUpdateCharacterStatus(FString StatusName, int32 StatusValue, int32 StatusDurationMinutes)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnAddOrUpdateCharacterStatusResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/AddOrUpdateCharacterStatus"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&StatusName=")) + StatusName
			+ FString(TEXT("&StatusValue=")) + FString::FromInt(StatusValue)
			+ FString(TEXT("&StatusDurationMinutes=")) + FString::FromInt(StatusDurationMinutes)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnAddOrUpdateCharacterStatusResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		/*TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

		}
		else
		{
		UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCharacterStatusResponseReceived Server returned no data!"));
		}*/
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCharacterStatusResponseReceived Error accessing server!"));
	}
}



void AOWSCharacter::GetCustomCharacterData()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetCustomCharacterDataResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetCustomCharacterData"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			 + FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
					tempCustomData.CustomFieldName = tempRow->GetStringField("CustomFieldName");
					tempCustomData.FieldValue = tempRow->GetStringField("FieldValue");

					CustomCharacterData.Add(tempCustomData);
				}
			}

			OnGetCustomCharacterData(CustomCharacterData);
			//NotifyGetCustomCharacterData(CustomCharacterData);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetCharacterStatusesResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetCharacterStatusesResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::NotifyGetCustomCharacterData_Implementation(const TArray<FCustomCharacterDataStruct> &CustomCharacterData) {

}

void AOWSCharacter::ErrorGetCustomCharacterData_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::AddOrUpdateCustomCharacterData(FString CustomFieldName, FString CustomValue)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnAddOrUpdateCustomCharacterDataResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/AddOrUpdateCustomCharacterData"));
			
		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&FieldName=")) + CustomFieldName
			+ FString(TEXT("&FieldValue=")) + CustomValue
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		UE_LOG(LogTemp, Log, TEXT("OnAddOrUpdateCustomCharacterDataResponseReceived - PlayerName: %s, FieldName: %s, Value: %s"), *PlayerName, *CustomFieldName, *CustomValue);

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnAddOrUpdateCustomCharacterDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			UE_LOG(OWS, Verbose, TEXT("OnAddOrUpdateCustomCharacterDataResponseReceived Success!"));
		}
		else
		{
		UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCustomCharacterDataResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnAddOrUpdateCustomCharacterDataResponseReceived Error accessing server!"));
	}
}





void AOWSCharacter::GetInventoryItems(FString InventoryName)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetInventoryItemsResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/GetInventoryItems"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetInventoryItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	int32 InventorySize = 1.f;

	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FInventoryItemStruct> InventoryItems;
			FName InventoryName;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				/*for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FInventoryItemStruct tempInventoryItem;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();
					tempInventoryItem.ItemName = tempRow->GetStringField("ItemName");
					tempInventoryItem.InventoryName = tempRow->GetStringField("InventoryName");
					InventoryName = FName(*tempInventoryItem.InventoryName);
					InventorySize = tempRow->GetIntegerField("InventorySize");

					//Default row when an inventory is empty just to get the inventory name
					if (tempInventoryItem.ItemName == "EmptyInventory")
						continue;

					tempInventoryItem.ItemDescription = tempRow->GetStringField("ItemDescription");
					FString tempUniqueItemGUIDString = tempRow->GetStringField("CharInventoryItemGUID");
					FGuid::Parse(tempUniqueItemGUIDString, tempInventoryItem.UniqueItemGUID);
					tempInventoryItem.InSlotNumber = tempRow->GetIntegerField("InSlotNumber");
					tempInventoryItem.Quantity = tempRow->GetIntegerField("Quantity");
					tempInventoryItem.ItemCanStack = tempRow->GetBoolField("ItemCanStack");
					tempInventoryItem.ItemStackSize = tempRow->GetIntegerField("ItemStackSize");
					tempInventoryItem.IsUsable = tempRow->GetBoolField("ItemIsUsable");
					tempInventoryItem.IsConsumedOnUse = tempRow->GetBoolField("ItemIsConsumedOnUse");
					tempInventoryItem.NumberOfUsesLeft = tempRow->GetIntegerField("NumberOfUsesLeft");
					tempInventoryItem.ItemWeight = (float)tempRow->GetNumberField("ItemWeight");
					tempInventoryItem.ItemTypeID = tempRow->GetIntegerField("ItemTypeID");
					tempInventoryItem.ItemTypeDescription = tempRow->GetStringField("ItemTypeDesc");
					tempInventoryItem.ItemTypeQuality = tempRow->GetIntegerField("ItemTypeQuality");
					
					tempInventoryItem.UserItemType = tempRow->GetIntegerField("UserItemType");
					tempInventoryItem.EquipmentType = tempRow->GetIntegerField("EquipmentType");
					tempInventoryItem.EquipmentSlotType = tempRow->GetIntegerField("EquipmentSlotType");
					tempInventoryItem.ItemTier = tempRow->GetIntegerField("ItemTier");
					tempInventoryItem.ItemQuality = tempRow->GetIntegerField("ItemQuality");
					tempInventoryItem.ItemDuration = tempRow->GetIntegerField("ItemDuration");
					tempInventoryItem.CanBeDropped = tempRow->GetBoolField("CanBeDropped");
					tempInventoryItem.CanBeDestroyed = tempRow->GetBoolField("CanBeDestroyed");

					tempInventoryItem.CustomData = tempRow->GetStringField("CustomData");
					tempInventoryItem.PerInstanceCustomData = tempRow->GetStringField("PerInstanceCustomData");
					tempInventoryItem.Condition = tempRow->GetIntegerField("Condition");
					//tempInventoryItem.ItemMesh = tempRow->GetStringField("ItemMesh");
					//tempInventoryItem.MeshToUseForPickup = tempRow->GetStringField("MeshToUseForPickup");
					tempInventoryItem.PremiumCurrencyPrice = tempRow->GetIntegerField("PremiumCurrencyPrice");
					tempInventoryItem.FreeCurrencyPrice = tempRow->GetIntegerField("FreeCurrencyPrice");

					tempInventoryItem.ItemMeshID = tempRow->GetIntegerField("ItemMeshID");
					tempInventoryItem.WeaponActorClassPath = tempRow->GetStringField("WeaponActorClass");
					tempInventoryItem.StaticMeshPath = tempRow->GetStringField("StaticMesh");
					tempInventoryItem.SkeletalMeshPath = tempRow->GetStringField("SkeletalMesh");

					//TextureIcon
					tempInventoryItem.TextureToUseForIcon = tempRow->GetStringField("TextureToUseForIcon");
					tempInventoryItem.TextureIcon = nullptr;
					if (!tempRow->GetStringField("TextureToUseForIcon").IsEmpty())
					{
						tempInventoryItem.TextureIcon = LoadObject<UTexture2D>(NULL, *tempRow->GetStringField("TextureToUseForIcon"), NULL, LOAD_None, NULL);

						if (!tempInventoryItem.TextureIcon)
						{
							UE_LOG(LogTemp, Error, TEXT("Error loading Texture Icon!"));
						}
					}

					tempInventoryItem.IconSlotWidth = tempRow->GetIntegerField("IconSlotWidth");
					tempInventoryItem.IconSlotHeight = tempRow->GetIntegerField("IconSlotHeight");

					if (tempInventoryItem.IconSlotWidth < 1)
						tempInventoryItem.IconSlotWidth = 1;

					if (tempInventoryItem.IconSlotHeight < 1)
						tempInventoryItem.IconSlotHeight = 1;

					InventoryItems.Add(tempInventoryItem);
				}*/

				//Send in JSON and get back InventoryName, InventorySize, and InventoryItems
				ReadInventoryItems(Rows, InventoryName, InventorySize, InventoryItems);
			}

			if (bShouldAutoLoadInventoriesToManage)
			{
				//Make sure the inventory size is in a valid range:
				if (InventorySize < 1)
					InventorySize = 1;
				if (InventorySize > 128)
					InventorySize = 128;

				//If found in InventoriesToManage, use that
				UOWSInventory* InventoryToFill = GetHUDInventoryFromName(InventoryName);
				if (InventoryToFill)
				{
					if (InventoryItems.Num() > 0)
					{
						InventoryToFill->AddItemsFromInventoryItemStruct(InventoryItems);
					}
				}
				else //Create the inventory
				{
					UOWSInventory* NewlyCreatedInventory = CreateHUDInventory(InventoryName, InventorySize, FMath::FloorToInt(FMath::Sqrt((float)InventorySize)));
					if (InventoryItems.Num() > 0)
					{
						NewlyCreatedInventory->AddItemsFromInventoryItemStruct(InventoryItems);
					}
				}

				//GetOWSPlayerController()->SynchUpLocalMeshItemsMap();
			}

			GetInventoryItemsComplete(InventoryName.ToString(), InventoryItems, InventorySize);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetInventoryItemsResponseReceived Server returned no data!"));
			GetInventoryItemsError(TEXT("OnGetInventoryItemsResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetInventoryItemsResponseReceived Error accessing server!"));
		GetInventoryItemsError(TEXT("OnGetInventoryItemsResponseReceived Error accessing server!"));
	}
}

void AOWSCharacter::ReadInventoryItems(const TArray<TSharedPtr<FJsonValue>> Rows, FName& InventoryName, int32& InventorySize, TArray<FInventoryItemStruct>& InventoryItems)
{
	for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
		FInventoryItemStruct tempInventoryItem;
		TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();
		tempInventoryItem.ItemName = tempRow->GetStringField("ItemName");
		tempInventoryItem.InventoryName = tempRow->GetStringField("InventoryName");
		InventoryName = FName(*tempInventoryItem.InventoryName);
		InventorySize = tempRow->GetIntegerField("InventorySize");

		//Default row when an inventory is empty just to get the inventory name
		if (tempInventoryItem.ItemName == "EmptyInventory")
			continue;

		tempInventoryItem.ItemDescription = tempRow->GetStringField("ItemDescription");
		FString tempUniqueItemGUIDString = tempRow->GetStringField("CharInventoryItemGUID");
		FGuid::Parse(tempUniqueItemGUIDString, tempInventoryItem.UniqueItemGUID);
		tempInventoryItem.InSlotNumber = tempRow->GetIntegerField("InSlotNumber");
		tempInventoryItem.Quantity = tempRow->GetIntegerField("Quantity");
		tempInventoryItem.ItemCanStack = tempRow->GetBoolField("ItemCanStack");
		tempInventoryItem.ItemStackSize = tempRow->GetIntegerField("ItemStackSize");
		tempInventoryItem.IsUsable = tempRow->GetBoolField("ItemIsUsable");
		tempInventoryItem.IsConsumedOnUse = tempRow->GetBoolField("ItemIsConsumedOnUse");
		tempInventoryItem.NumberOfUsesLeft = tempRow->GetIntegerField("NumberOfUsesLeft");
		tempInventoryItem.ItemWeight = (float)tempRow->GetNumberField("ItemWeight");
		tempInventoryItem.ItemTypeID = tempRow->GetIntegerField("ItemTypeID");
		tempInventoryItem.ItemTypeDescription = tempRow->GetStringField("ItemTypeDesc");
		tempInventoryItem.ItemTypeQuality = tempRow->GetIntegerField("ItemTypeQuality");

		tempInventoryItem.UserItemType = tempRow->GetIntegerField("UserItemType");
		tempInventoryItem.EquipmentType = tempRow->GetIntegerField("EquipmentType");
		tempInventoryItem.EquipmentSlotType = tempRow->GetIntegerField("EquipmentSlotType");
		tempInventoryItem.ItemTier = tempRow->GetIntegerField("ItemTier");
		tempInventoryItem.ItemQuality = tempRow->GetIntegerField("ItemQuality");
		tempInventoryItem.ItemDuration = tempRow->GetIntegerField("ItemDuration");
		tempInventoryItem.CanBeDropped = tempRow->GetBoolField("CanBeDropped");
		tempInventoryItem.CanBeDestroyed = tempRow->GetBoolField("CanBeDestroyed");

		tempInventoryItem.CustomData = tempRow->GetStringField("CustomData");
		tempInventoryItem.PerInstanceCustomData = tempRow->GetStringField("PerInstanceCustomData");
		tempInventoryItem.Condition = tempRow->GetIntegerField("Condition");
		//tempInventoryItem.ItemMesh = tempRow->GetStringField("ItemMesh");
		//tempInventoryItem.MeshToUseForPickup = tempRow->GetStringField("MeshToUseForPickup");
		tempInventoryItem.PremiumCurrencyPrice = tempRow->GetIntegerField("PremiumCurrencyPrice");
		tempInventoryItem.FreeCurrencyPrice = tempRow->GetIntegerField("FreeCurrencyPrice");

		tempInventoryItem.ItemMeshID = tempRow->GetIntegerField("ItemMeshID");
		tempInventoryItem.WeaponActorClassPath = tempRow->GetStringField("WeaponActorClass");
		tempInventoryItem.StaticMeshPath = tempRow->GetStringField("StaticMesh");
		tempInventoryItem.SkeletalMeshPath = tempRow->GetStringField("SkeletalMesh");

		//TextureIcon
		tempInventoryItem.TextureToUseForIcon = tempRow->GetStringField("TextureToUseForIcon");
		tempInventoryItem.TextureIcon = nullptr;
		if (!tempRow->GetStringField("TextureToUseForIcon").IsEmpty())
		{
			tempInventoryItem.TextureIcon = LoadObject<UTexture2D>(NULL, *tempRow->GetStringField("TextureToUseForIcon"), NULL, LOAD_None, NULL);

			if (!tempInventoryItem.TextureIcon)
			{
				UE_LOG(LogTemp, Error, TEXT("Error loading Texture Icon!"));
			}
		}

		tempInventoryItem.IconSlotWidth = tempRow->GetIntegerField("IconSlotWidth");
		tempInventoryItem.IconSlotHeight = tempRow->GetIntegerField("IconSlotHeight");

		if (tempInventoryItem.IconSlotWidth < 1)
			tempInventoryItem.IconSlotWidth = 1;

		if (tempInventoryItem.IconSlotHeight < 1)
			tempInventoryItem.IconSlotHeight = 1;

		InventoryItems.Add(tempInventoryItem);
	}
}


//This is an example showing how you can override a BP event in C++.  This will not be called if the BP event node exists in the BP.
void AOWSCharacter::GetInventoryItemsComplete_Implementation(const FString &InventoryName, const TArray<FInventoryItemStruct> &InventoryItems, int32 InventorySize)
{
	UE_LOG(OWS, Error, TEXT("GetInventoryItemsComplete_Implementation called!"));

}

void AOWSCharacter::GetInventoryItemsError_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::AddItemToInventory(FString InventoryName, FString ItemName, int InSlotNumber, int Quantity, int NumberOfUsesLeft, int Condition, FGuid &UniqueItemGUID)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		UniqueItemGUID = FGuid::NewGuid();

		Client_AddItemToInventory(FName(*InventoryName), ItemName, 1, InSlotNumber, NumberOfUsesLeft, Condition, "", UniqueItemGUID, 0);

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnAddItemToInventoryResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/AddItemToInventory"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName
			+ FString(TEXT("&UniqueItemGUID=")) + UniqueItemGUID.ToString()
			+ FString(TEXT("&ItemName=")) + ItemName
			+ FString(TEXT("&InSlotNumber=")) + FString::FromInt(InSlotNumber)
			+ FString(TEXT("&Quantity=")) + FString::FromInt(Quantity)
			+ FString(TEXT("&NumberOfUsesLeft=")) + FString::FromInt(NumberOfUsesLeft)
			+ FString(TEXT("&Condition=")) + FString::FromInt(Condition)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnAddItemToInventoryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			AddItemToInventoryComplete();
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnAddItemToInventoryResponseReceived Server returned no data!"));
			AddItemToInventoryError();
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnAddItemToInventoryResponseReceived Error accessing server!"));
		AddItemToInventoryError();
	}
}


void AOWSCharacter::AddItemToInventoryComplete_Implementation()
{

}

void AOWSCharacter::AddItemToInventoryError_Implementation()
{

}

void AOWSCharacter::AddItemToInventoryWithCustomData(FString InventoryName, FString ItemName, int InSlotNumber, int Quantity, int NumberOfUsesLeft, int Condition, FString CustomData, FGuid &UniqueItemGUID)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		UniqueItemGUID = FGuid::NewGuid();

		Client_AddItemToInventory(FName(*InventoryName), ItemName, 1, InSlotNumber, NumberOfUsesLeft, Condition, CustomData, UniqueItemGUID, 0);

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnAddItemToInventoryWithCustomDataResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/AddItemToInventoryWithCustomData"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName
			+ FString(TEXT("&UniqueItemGUID=")) + UniqueItemGUID.ToString()
			+ FString(TEXT("&ItemName=")) + ItemName
			+ FString(TEXT("&InSlotNumber=")) + FString::FromInt(InSlotNumber)
			+ FString(TEXT("&Quantity=")) + FString::FromInt(Quantity)
			+ FString(TEXT("&NumberOfUsesLeft=")) + FString::FromInt(NumberOfUsesLeft)
			+ FString(TEXT("&Condition=")) + FString::FromInt(Condition)
			+ FString(TEXT("&CustomData=")) + CustomData
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnAddItemToInventoryWithCustomDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			AddItemToInventoryWithCustomDataComplete();
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnAddItemToInventoryWithCustomDataResponseReceived Server returned no data!"));
			AddItemToInventoryWithCustomDataError();
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnAddItemToInventoryWithCustomDataResponseReceived Error accessing server!"));
		AddItemToInventoryWithCustomDataError();
	}
}

void AOWSCharacter::AddItemToInventoryWithCustomDataComplete_Implementation() {

}

void AOWSCharacter::AddItemToInventoryWithCustomDataError_Implementation() {

}

void AOWSCharacter::RemoveItemFromInventory(FGuid UniqueItemGUID)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnRemoveItemFromInventoryResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/RemoveItemFromInventory"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&UniqueItemGUID=")) + UniqueItemGUID.ToString()
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnRemoveItemFromInventoryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnRemoveItemFromInventoryResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnRemoveItemFromInventoryResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::SaveInventory(FString InventoryName, FString InventoryData)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnSaveInventoryResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/SaveInventory"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName
			+ FString(TEXT("&InventoryData=")) + InventoryData
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnSaveInventoryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnSaveInventoryResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSaveInventoryResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::SerializeAndSaveInventory(FName InventoryName)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnSerializeAndSaveInventoryResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/SaveInventory"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName.ToString()
			+ FString(TEXT("&InventoryData=")) + SerializeInventory(InventoryName)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnSerializeAndSaveInventoryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnSerializeAndSaveInventoryResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSerializeAndSaveInventoryResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::CreateInventory(FString InventoryName, int InventorySize)
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnSaveInventoryResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/CreateInventory"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&InventoryName=")) + InventoryName
			+ FString(TEXT("&InventorySize=")) + FString::FromInt(InventorySize)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnCreateInventoryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			CreateInventoryComplete();
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnSaveInventoryResponseReceived Server returned no data!"));
			CreateInventoryError(TEXT("OnSaveInventoryResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnSaveInventoryResponseReceived Error accessing server!"));
		CreateInventoryError(TEXT("OnSaveInventoryResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::CreateInventoryComplete_Implementation() {

}

void AOWSCharacter::CreateInventoryError_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::ParseInventoryCustomData(FString InventoryCustomData, TArray<FCustomInventoryItemDataStruct> &ItemCustomDataArray)
{
	TArray<FCustomInventoryItemDataStruct> tempCustomInventoryData;

	FCustomInventoryItemDataStruct tempInventoryItemData;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InventoryCustomData);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		for (auto It = JsonObject->Values.CreateConstIterator(); It; ++It)
		{
			tempInventoryItemData.CustomFieldName = It.Key();
			tempInventoryItemData.FieldValue = It.Value()->AsString();
			tempCustomInventoryData.Add(tempInventoryItemData);
		}

		ItemCustomDataArray = tempCustomInventoryData;
	}		
}


/***** Abilities *****/
void AOWSCharacter::GetCharacterAbilities()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetCharacterAbilitiesResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGAbility/CharacterAbilities"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetCharacterAbilitiesResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FAbilityStruct> Abilities;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FAbilityStruct tempAbility;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();

					tempAbility.AbilityName = tempRow->GetStringField("AbilityName");
					tempAbility.AbilityLevel = tempRow->GetNumberField("AbilityLevel");
					tempAbility.GameplayAbilityClassName = tempRow->GetStringField("GameplayAbilityClassName");
					tempAbility.TextureToUseForIcon = tempRow->GetStringField("TextureToUseForIcon");

					Abilities.Add(tempAbility);
				}
			}
			GetCharacterAbilitiesComplete(Abilities);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetCharacterAbilitiesResponseReceived Server returned no data!"));
			GetCharacterAbilitiesError(TEXT("OnGetCharacterAbilitiesResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetCharacterAbilitiesResponseReceived Error accessing server!"));
		GetCharacterAbilitiesError(TEXT("OnGetCharacterAbilitiesResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::GetCharacterAbilitiesComplete_Implementation(const TArray<FAbilityStruct> &AbilityBars) {

}

void AOWSCharacter::GetCharacterAbilitiesError_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::GetAbilityBars()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetAbilityBarsResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGAbility/GetAbilityBars"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetAbilityBarsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FAbilityBarStruct> AbilityBars;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				int32 CharAbilityBarID = 0;

				if (Rows.Num() > 0)
				{
					for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
						FAbilityBarStruct tempAbilityBar;
						TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();

						tempAbilityBar.AbilityBarID = tempRow->GetNumberField("CharAbilityBarID");
						tempAbilityBar.AbilityBarName = tempRow->GetStringField("AbilityBarName");
						tempAbilityBar.MaxNumberOfSlots = tempRow->GetNumberField("MaxNumberOfSlots");
						tempAbilityBar.NumberOfUnlockedSlots = tempRow->GetNumberField("NumberOfUnlockedSlots");
						tempAbilityBar.AbilityBarCustomJSON = tempRow->GetStringField("CharAbilityBarsCustomJSON");

						AbilityBars.Add(tempAbilityBar);
					}
				}
			}
			GetAbilityBarsComplete(AbilityBars);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetAbilityBarsResponseReceived Server returned no data!"));
			GetAbilityBarsError(TEXT("OnGetAbilityBarsResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetAbilityBarsResponseReceived Error accessing server!"));
		GetAbilityBarsError(TEXT("OnGetAbilityBarsResponseReceived Error accessing server!"));
	}
}


void AOWSCharacter::GetAbilityBarsComplete_Implementation(const TArray<FAbilityBarStruct> &AbilityBars) {

}

void AOWSCharacter::GetAbilityBarsError_Implementation(const FString &ErrorMsg) {

}


void AOWSCharacter::GetAbilityBarsAndAbilities()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacter::OnGetAbilityBarsAndAbilitiesResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGAbility/GetAbilityBarsAndAbilities"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacter::OnGetAbilityBarsAndAbilitiesResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FAbilityBarStruct> AbilityBars;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				int32 CharAbilityBarID = 0;

				FAbilityBarStruct tempAbilityBar;
				FAbilityStruct tempAbility;

				if (Rows.Num() > 0)
				{
					TSharedPtr<FJsonObject> tempFirstRow = Rows[0]->AsObject();
					CharAbilityBarID = tempFirstRow->GetNumberField("CharAbilityBarID");

					for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
						TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();

						tempAbilityBar.AbilityBarID = tempRow->GetNumberField("CharAbilityBarID");
						tempAbilityBar.AbilityBarCustomJSON = tempRow->GetStringField("CharAbilityBarsCustomJSON");

						if (CharAbilityBarID != tempAbilityBar.AbilityBarID)
						{
							AbilityBars.Add(tempAbilityBar);
							tempAbilityBar.Abilities.Empty(0);
						}

						tempAbilityBar.AbilityBarName = tempRow->GetStringField("AbilityBarName");
						tempAbilityBar.MaxNumberOfSlots = tempRow->GetNumberField("MaxNumberOfSlots");
						tempAbilityBar.NumberOfUnlockedSlots = tempRow->GetNumberField("NumberOfUnlockedSlots");
						tempAbilityBar.AbilityBarCustomJSON = tempRow->GetStringField("CharAbilityBarsCustomJSON");

						tempAbility.AbilityName = tempRow->GetStringField("AbilityName");
						tempAbility.AbilityLevel = tempRow->GetNumberField("AbilityLevel");
						tempAbility.AbilityInSlotNumber = tempRow->GetNumberField("InSlotNumber");
						tempAbility.GameplayAbilityClassName = tempRow->GetStringField("GameplayAbilityClassName");
						tempAbility.TextureToUseForIcon = tempRow->GetStringField("TextureToUseForIcon");

						tempAbility.AbilityCustomJSON = tempRow->GetStringField("AbilityCustomJSON");
						tempAbility.CharHasAbilitiesCustomJSON = tempRow->GetStringField("CharHasAbilitiesCustomJSON");

						tempAbilityBar.Abilities.Add(tempAbility);
						
						CharAbilityBarID = tempAbilityBar.AbilityBarID;
					}

					AbilityBars.Add(tempAbilityBar);
				}
			}
			GetAbilityBarsAndAbilitiesComplete(AbilityBars);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnGetAbilityBarsAndAbilitiesResponseReceived Server returned no data!"));
			GetAbilityBarsAndAbilitiesError(TEXT("OnGetAbilityBarsAndAbilitiesResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnGetAbilityBarsAndAbilitiesResponseReceived Error accessing server!"));
		GetAbilityBarsAndAbilitiesError(TEXT("OnGetAbilityBarsAndAbilitiesResponseReceived Error accessing server!"));
	}
}

void AOWSCharacter::GetAbilityBarsAndAbilitiesComplete_Implementation(const TArray<FAbilityBarStruct> &AbilityBars) {

}

void AOWSCharacter::GetAbilityBarsAndAbilitiesError_Implementation(const FString &ErrorMsg) {

}


AOWSGameMode* AOWSCharacter::GetGameMode()
{
	return (AOWSGameMode*)GetWorld()->GetAuthGameMode();
}


//HUD Inventory System
void AOWSCharacter::OnRep_InventoriesToManage()
{
	if (InventoriesToManage.Num() < 1)
	{
		UE_LOG(OWS, Error, TEXT("OnRep_InventoriesToManage.Num() < 1"));
		return;
	}

	if (InventoriesToManage[0])
	{
		UE_LOG(OWS, Error, TEXT("OnRep_InventoriesToManage First Entry is Valid!"));
		return;
	}

	UE_LOG(OWS, Error, TEXT("OnRep_InventoriesToManage"));
}

UOWSInventory* AOWSCharacter::CreateHUDInventory(FName InventoryName, int32 Size, int32 NumberOfColumns)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UOWSInventory* Inventory = NewObject<UOWSInventory>();
		Inventory->SetInventoryName(InventoryName);
		Inventory->SetInventorySize(Size, NumberOfColumns);
		Inventory->SetOwningPlayerCharacter(this);
		InventoriesToManage.Add(Inventory);
		Client_CreateHUDInventory(InventoryName, Size, NumberOfColumns);
		return Inventory;
	}

	return nullptr;
}

void AOWSCharacter::Client_CreateHUDInventory_Implementation(FName InventoryName, int32 Size, int32 NumberOfColumns)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UOWSInventory* Inventory = NewObject<UOWSInventory>();
		Inventory->SetInventoryName(InventoryName);
		Inventory->SetInventorySize(Size, NumberOfColumns);
		Inventory->SetOwningPlayerCharacter(this);
		InventoriesToManage.Add(Inventory);
	}
}

void AOWSCharacter::Client_AddItemToInventory_Implementation(const FName& InventoryName, const FString& ItemName, const int32 StackSize, const int32 InSlotNumber, const int32 NumberOfUsesLeft, const int32 Condition,
	const FString& PerInstanceCustomData, const FGuid UniqueItemGUID, const int32 ItemMeshID)
{
	AOWSInventoryItem* Item = NewObject<AOWSInventoryItem>();
	Item->ItemName = ItemName;
	Item->StackSize = StackSize;
	Item->NumberOfUsesLeft = NumberOfUsesLeft;
	Item->Condition = Condition;
	Item->PerInstanceCustomData = PerInstanceCustomData;
	Item->UniqueItemGUID = UniqueItemGUID;
	Item->ItemMeshID = ItemMeshID;

	auto FoundEntry = LocalInventoryItems.FindByPredicate([&](FInventoryItemStruct& InItem)
	{
		return InItem.ItemName == ItemName;
	});

	if (FoundEntry)
	{
		AOWSPlayerController* PlayerController = Cast<AOWSPlayerController>(GetController());

		Item->IconTexture = PlayerController->LoadTextureReference(FoundEntry->TextureToUseForIcon);
		Item->IconSlotWidth = FoundEntry->IconSlotWidth;
		Item->IconSlotHeight = FoundEntry->IconSlotHeight;
	}

	UOWSInventory* FoundInventory = GetHUDInventoryFromName(InventoryName);

	if (FoundInventory)
	{
		FoundInventory->AddItemToSlot_Internal(Item, InSlotNumber);
	}
}

bool AOWSCharacter::AddItemToLocalInventoryItems(const FString& ItemName, const bool ItemCanStack, const bool IsUsable, const bool IsConsumedOnUse, const int32 ItemTypeID,
	const FString& TextureToUseForIcon, const int32 IconSlotWidth, const int32 IconSlotHeight, const int32 ItemMeshID, const FString& CustomData)
{
	auto FoundEntry = LocalInventoryItems.FindByPredicate([&](FInventoryItemStruct& InItem)
	{
		return InItem.ItemName == ItemName;
	});

	if (FoundEntry)
		return false;

	FInventoryItemStruct tempItem;

	tempItem.ItemName = ItemName;
	tempItem.ItemCanStack = ItemCanStack;
	tempItem.IsUsable = IsUsable;
	tempItem.IsConsumedOnUse = IsConsumedOnUse;
	tempItem.ItemTypeID = ItemTypeID;
	tempItem.TextureToUseForIcon = TextureToUseForIcon;
	tempItem.IconSlotWidth = IconSlotWidth;
	tempItem.IconSlotHeight = IconSlotHeight;
	tempItem.ItemMeshID = ItemMeshID;
	tempItem.CustomData = CustomData;

	LocalInventoryItems.Add(tempItem);

	return true;
}

void AOWSCharacter::Client_AddItemToLocalInventoryItems_Implementation(const FString& ItemName, const bool ItemCanStack, const bool IsUsable, const bool IsConsumedOnUse, const int32 ItemTypeID,
	const FString& TextureToUseForIcon, const int32 IconSlotWidth, const int32 IconSlotHeight, const int32 ItemMeshID, const FString& CustomData)
{
	FInventoryItemStruct tempItem;

	tempItem.ItemName = ItemName;
	tempItem.ItemCanStack = ItemCanStack;
	tempItem.IsUsable = IsUsable;
	tempItem.IsConsumedOnUse = IsConsumedOnUse;
	tempItem.ItemTypeID = ItemTypeID;
	tempItem.TextureToUseForIcon = TextureToUseForIcon;
	tempItem.IconSlotWidth = IconSlotWidth;
	tempItem.IconSlotHeight = IconSlotHeight;
	tempItem.ItemMeshID = ItemMeshID;
	tempItem.CustomData = CustomData;

	LocalInventoryItems.Add(tempItem);
}

UOWSInventory* AOWSCharacter::GetHUDInventoryFromName(FName InventoryName)
{
	if (InventoriesToManage.Num() < 1)
		return nullptr;

	/*
	for (TArray<UOWSInventory*>::TConstIterator InventoryIter(InventoriesToManage); InventoryIter; ++InventoryIter)
	{
		if ((*InventoryIter)->InventoryName == InventoryName)
		{
			return (*InventoryIter);
		}
	}*/

	/*for (auto CurInventory : InventoriesToManage)
	{
		if (CurInventory->InventoryName == InventoryName)
		{
			return CurInventory;
		}
	}*/

	auto FoundEntry = InventoriesToManage.FindByPredicate([&](UOWSInventory* InItem)
	{
		//return InItem->Choices.Contains(InputString);
		return InItem != nullptr && InItem->InventoryName == InventoryName;
	});

	if (FoundEntry)
	{
		return (*FoundEntry);
	}
	else
	{
		return nullptr;
	}
}


FString AOWSCharacter::SerializeInventory(FName InventoryName)
{
	FString output = "";

	UOWSInventory* InventoryToSerialize = GetHUDInventoryFromName(InventoryName);

	if (InventoryToSerialize)
	{
		//Loop through the ItemStacks and serialize them to a string
		for (auto ItemStack : InventoryToSerialize->InventoryItemStacks)
		{
			AOWSInventoryItem* InventoryItem = ItemStack->GetTopItemFromStack();

			//Only save valid items
			if (InventoryItem)
			{
				output += InventoryItem->UniqueItemGUID.ToString() + "*" + FString::FromInt(ItemStack->SlotNumber) + "*" + FString::FromInt(ItemStack->InventoryItems.Num()) + "*" + FString::FromInt(InventoryItem->NumberOfUsesLeft) + "*" + FString::FromInt(InventoryItem->Condition);
				output += "|";
			}
		}

		//Remove the trailing "|"
		if (output.Len() > 1)
		{
			output = output.Left(output.Len() - 1);
		}
	}

	return output;
}


void AOWSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AOWSCharacter, CharacterName);
	DOREPLIFETIME(AOWSCharacter, ClassName);	
	DOREPLIFETIME(AOWSCharacter, Gender);
	DOREPLIFETIME(AOWSCharacter, IsEnemy);
	DOREPLIFETIME(AOWSCharacter, CharacterLevel);
	DOREPLIFETIME(AOWSCharacter, XP);
	DOREPLIFETIME(AOWSCharacter, HitDice);
	DOREPLIFETIME(AOWSCharacter, HitDie);
	DOREPLIFETIME(AOWSCharacter, MaxHP);
	DOREPLIFETIME(AOWSCharacter, Wounds);
	DOREPLIFETIME(AOWSCharacter, Strength);
	DOREPLIFETIME(AOWSCharacter, Dexterity);
	DOREPLIFETIME(AOWSCharacter, Constitution);
	DOREPLIFETIME(AOWSCharacter, Intellect);
	DOREPLIFETIME(AOWSCharacter, Wisdom);
	DOREPLIFETIME(AOWSCharacter, Charisma);
	DOREPLIFETIME(AOWSCharacter, Spirit);
	DOREPLIFETIME(AOWSCharacter, Magic);
	DOREPLIFETIME(AOWSCharacter, Fortitude);
	DOREPLIFETIME(AOWSCharacter, Reflex);
	DOREPLIFETIME(AOWSCharacter, Willpower);
	DOREPLIFETIME(AOWSCharacter, TeamNumber);
	DOREPLIFETIME(AOWSCharacter, Perception);
	DOREPLIFETIME(AOWSCharacter, Acrobatics);
	DOREPLIFETIME(AOWSCharacter, Climb);
	DOREPLIFETIME(AOWSCharacter, Stealth);
	DOREPLIFETIME(AOWSCharacter, Thirst);
	DOREPLIFETIME(AOWSCharacter, Hunger);
	DOREPLIFETIME(AOWSCharacter, Gold);
	DOREPLIFETIME(AOWSCharacter, Score);
	DOREPLIFETIME(AOWSCharacter, MaxHealth);
	DOREPLIFETIME(AOWSCharacter, Health);
	DOREPLIFETIME(AOWSCharacter, HealthRegenRate);
	DOREPLIFETIME(AOWSCharacter, MaxMana);
	DOREPLIFETIME(AOWSCharacter, Mana);
	DOREPLIFETIME(AOWSCharacter, ManaRegenRate);
	DOREPLIFETIME(AOWSCharacter, MaxEnergy);
	DOREPLIFETIME(AOWSCharacter, Energy);
	DOREPLIFETIME(AOWSCharacter, EnergyRegenRate);
	DOREPLIFETIME(AOWSCharacter, MaxFatigue);
	DOREPLIFETIME(AOWSCharacter, Fatigue);
	DOREPLIFETIME(AOWSCharacter, FatigueRegenRate);
	DOREPLIFETIME(AOWSCharacter, MaxStamina);
	DOREPLIFETIME(AOWSCharacter, Stamina);
	DOREPLIFETIME(AOWSCharacter, StaminaRegenRate);
	DOREPLIFETIME(AOWSCharacter, MaxEndurance);
	DOREPLIFETIME(AOWSCharacter, Endurance);
	DOREPLIFETIME(AOWSCharacter, EnduranceRegenRate);
	DOREPLIFETIME(AOWSCharacter, Agility);
	DOREPLIFETIME(AOWSCharacter, BaseAttack);
	DOREPLIFETIME(AOWSCharacter, BaseAttackBonus);
	DOREPLIFETIME(AOWSCharacter, AttackPower);
	DOREPLIFETIME(AOWSCharacter, AttackSpeed);
	DOREPLIFETIME(AOWSCharacter, CritChance);
	DOREPLIFETIME(AOWSCharacter, CritMultiplier);
	DOREPLIFETIME(AOWSCharacter, Haste);
	DOREPLIFETIME(AOWSCharacter, SpellPower);
	DOREPLIFETIME(AOWSCharacter, SpellPenetration);
	DOREPLIFETIME(AOWSCharacter, Defense);
	DOREPLIFETIME(AOWSCharacter, Dodge);
	DOREPLIFETIME(AOWSCharacter, Parry);
	DOREPLIFETIME(AOWSCharacter, Avoidance);
	DOREPLIFETIME(AOWSCharacter, Versatility);
	DOREPLIFETIME(AOWSCharacter, Multishot);
	DOREPLIFETIME(AOWSCharacter, Initiative);
	DOREPLIFETIME(AOWSCharacter, NaturalArmor);
	DOREPLIFETIME(AOWSCharacter, PhysicalArmor);
	DOREPLIFETIME(AOWSCharacter, BonusArmor);
	DOREPLIFETIME(AOWSCharacter, ForceArmor);
	DOREPLIFETIME(AOWSCharacter, MagicArmor);
	DOREPLIFETIME(AOWSCharacter, Resistance);
	DOREPLIFETIME(AOWSCharacter, ReloadSpeed);

	DOREPLIFETIME(AOWSCharacter, PremiumCurrency);
	DOREPLIFETIME(AOWSCharacter, FreeCurrency);
	DOREPLIFETIME(AOWSCharacter, Silver);
	DOREPLIFETIME(AOWSCharacter, Copper);

	DOREPLIFETIME(AOWSCharacter, Range);
	DOREPLIFETIME(AOWSCharacter, Speed);

	DOREPLIFETIME(AOWSCharacter, IsAdmin);
	DOREPLIFETIME(AOWSCharacter, IsModerator);

	//DOREPLIFETIME_CONDITION(AOWSCharacter, InventoriesToManage, COND_OwnerOnly)
}


void AOWSCharacter::OnGetCustomCharacterData(const TArray<FCustomCharacterDataStruct> &CustomCharacterData) 
{
	UE_LOG(OWS, Verbose, TEXT("OnGetCustomCharacterData Called"));

	NotifyGetCustomCharacterData(CustomCharacterData);
}