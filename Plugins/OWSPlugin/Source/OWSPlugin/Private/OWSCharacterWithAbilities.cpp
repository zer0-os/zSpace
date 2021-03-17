// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSCharacterWithAbilities.h"
#include "OWSPlugin.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Core/Public/Misc/Guid.h"
#include "OWSPlayerController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsModule.h"
#include "OWSAttributeSet.h"
//#include "OWSGameplayAbility.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

class AOWSAdvancedProjectile;

#define CONSTRUCT_CLASS(Class, Name) Class* Name = NewObject<Class>(GetTransientPackage(), FName(TEXT(#Name)))

FName AOWSCharacterWithAbilities::AbilitySystemComponentName(TEXT("AbilitySystem"));

AOWSCharacterWithAbilities::AOWSCharacterWithAbilities(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(AOWSCharacterWithAbilities::AbilitySystemComponentName);
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SpellAbilityHandles.SetNum(22, false);
	WeaponAbilityHandles.SetNum(22, false);

	OWSAttributes = CreateDefaultSubobject<UOWSAttributeSet>(TEXT("AttributeSet"));
}


void AOWSCharacterWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	//OWSAttributes = NewObject<UOWSAttributeSet>();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsAMob)
		{
			OnOWSAttributeInitalizationComplete();
		}
	}

	SetupAttributeChangeDelegates();
}

void AOWSCharacterWithAbilities::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*AbilitySystem->InitStats(
		UOWSAttributeSet::StaticClass(), NULL);*/
}

void AOWSCharacterWithAbilities::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));
	AbilitySystem->RegisterGenericGameplayTagEvent().AddUObject(this, &AOWSCharacterWithAbilities::OnGameplayEffectTagCountChanged);
}

void AOWSCharacterWithAbilities::OnGameplayEffectTagCountChanged(const FGameplayTag GameplayTag, int32 TagCount)
{
	UE_LOG(OWS, Verbose, TEXT("AOWSCharacterWithAbilities: OnGameplayEffectTagCountChanged - %s - %d"), *GameplayTag.GetTagName().ToString(), TagCount);
}

void AOWSCharacterWithAbilities::ChangeSpell(int SpellSlot, TSubclassOf<class UGameplayAbility> NewSpell)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewSpell && SpellSlot > 0 && SpellSlot <= SpellAbilityHandles.Num())
			{
				AbilitySystem->ClearAbility(SpellAbilityHandles[SpellSlot - 1]);

				SpellAbilityHandles[SpellSlot - 1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewSpell.GetDefaultObject(), 1, SpellSlot - 1));
			}
		}
	}
}

void AOWSCharacterWithAbilities::ChangeWeapon(int WeaponSlot, TSubclassOf<class UGameplayAbility> NewWeapon)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewWeapon && WeaponSlot > 0 && WeaponSlot <= WeaponAbilityHandles.Num())
			{
				AbilitySystem->ClearAbility(WeaponAbilityHandles[WeaponSlot - 1]);

				WeaponAbilityHandles[WeaponSlot - 1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewWeapon.GetDefaultObject(), 1, WeaponSlot + 21));
			}
		}
	}
}

void AOWSCharacterWithAbilities::ClearAbility(int SpellSlot)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (SpellSlot > 0 && SpellSlot < SpellAbilityHandles.Num())
			{
				AbilitySystem->ClearAbility(SpellAbilityHandles[SpellSlot - 1]);
			}
		}
	}
}

void AOWSCharacterWithAbilities::GrantAbility(TSubclassOf<class UGameplayAbility> NewAbility)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewAbility.GetDefaultObject(), 1));
			}
		}
	}
}

void AOWSCharacterWithAbilities::GrantAbilityKeyBind(TSubclassOf<class UGameplayAbility> NewAbility, int AbilityLevel, int InputID)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewAbility.GetDefaultObject(), AbilityLevel, InputID));
			}
		}
	}
}

void AOWSCharacterWithAbilities::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Initialize our abilities
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
		AddDefaultGameplayAbilities();
	}
}

void AOWSCharacterWithAbilities::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystem)
	{
		AbilitySystem->RefreshAbilityActorInfo();
	}
}

void AOWSCharacterWithAbilities::AddDefaultGameplayAbilities()
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (Ability1)
			{
				SpellAbilityHandles[0] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability1.GetDefaultObject(), 1, 0));
			}

			if (Ability2)
			{
				SpellAbilityHandles[1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability2.GetDefaultObject(), 1, 1));
			}

			if (Ability3)
			{
				SpellAbilityHandles[2] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability3.GetDefaultObject(), 1, 2));
			}

			if (Ability4)
			{
				SpellAbilityHandles[3] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability4.GetDefaultObject(), 1, 3));
			}

			if (Ability5)
			{
				SpellAbilityHandles[4] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability5.GetDefaultObject(), 1, 4));
			}

			if (Ability6)
			{
				SpellAbilityHandles[5] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability6.GetDefaultObject(), 1, 5));
			}

			if (Ability7)
			{
				SpellAbilityHandles[6] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability7.GetDefaultObject(), 1, 6));
			}

			if (Ability8)
			{
				SpellAbilityHandles[7] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability8.GetDefaultObject(), 1, 7));
			}

			if (Ability9)
			{
				SpellAbilityHandles[8] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability9.GetDefaultObject(), 1, 8));
			}

			if (Ability0)
			{
				SpellAbilityHandles[9] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability0.GetDefaultObject(), 1, 9));
			}
			
			if (Ability11)
			{
				SpellAbilityHandles[10] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability11.GetDefaultObject(), 1, 10));
			}
			if (Ability12)
			{
				SpellAbilityHandles[11] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability12.GetDefaultObject(), 1, 11));
			}
			if (Ability13)
			{
				SpellAbilityHandles[12] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability13.GetDefaultObject(), 1, 12));
			}
			if (Ability14)
			{
				SpellAbilityHandles[13] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability14.GetDefaultObject(), 1, 13));
			}
			if (Ability15)
			{
				SpellAbilityHandles[14] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability15.GetDefaultObject(), 1, 14));
			}
			if (Ability16)
			{
				SpellAbilityHandles[15] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability16.GetDefaultObject(), 1, 15));
			}
			if (Ability17)
			{
				SpellAbilityHandles[16] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability17.GetDefaultObject(), 1, 16));
			}
			if (Ability18)
			{
				SpellAbilityHandles[17] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability18.GetDefaultObject(), 1, 17));
			}
			if (Ability19)
			{
				SpellAbilityHandles[18] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability19.GetDefaultObject(), 1, 18));
			}
			if (Ability20)
			{
				SpellAbilityHandles[19] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability20.GetDefaultObject(), 1, 19));
			}
			if (Ability21)
			{
				SpellAbilityHandles[20] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability21.GetDefaultObject(), 1, 20));
			}
			if (Ability22)
			{
				SpellAbilityHandles[21] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability22.GetDefaultObject(), 1, 21));
			}

			if (Weapon1)
			{
				WeaponAbilityHandles[0] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Weapon1.GetDefaultObject(), 1, 22));
			}

			if (Weapon2)
			{
				WeaponAbilityHandles[1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Weapon2.GetDefaultObject(), 1, 23));
			}

		}
	}
}

void AOWSCharacterWithAbilities::SetupAttributeChangeDelegates()
{
	if (AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UOWSAttributeSet::GetHealthAttribute()).AddUObject(this, &AOWSCharacterWithAbilities::OnHealthChange);
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UOWSAttributeSet::GetEnergyAttribute()).AddUObject(this, &AOWSCharacterWithAbilities::OnEnergyChange);
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UOWSAttributeSet::GetManaAttribute()).AddUObject(this, &AOWSCharacterWithAbilities::OnManaChange);
	}
}

void AOWSCharacterWithAbilities::OnHealthChange(const FOnAttributeChangeData& Data)
{
	HealthChanged(Data.OldValue, Data.NewValue);
}

void AOWSCharacterWithAbilities::OnEnergyChange(const FOnAttributeChangeData& Data)
{
	EnergyChanged(Data.OldValue, Data.NewValue);
}

void AOWSCharacterWithAbilities::OnManaChange(const FOnAttributeChangeData& Data)
{
	ManaChanged(Data.OldValue, Data.NewValue);
}

void AOWSCharacterWithAbilities::HandleProjectileDamage(AOWSAdvancedProjectile* Projectile, bool UseExplosionEffect)
{
	FGameplayEffectSpec* Spec;
	if (UseExplosionEffect)
	{
		Spec = Projectile->AoEDamageEffectOnHit.Data.Get();;
	}
	else
	{
		Spec = Projectile->DamageEffectOnHit.Data.Get();;
	}

	if (!Spec)
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(OWS, Verbose, TEXT("Server: HandleProjectileDamage"));
		if (GetAbilitySystemComponent() != nullptr)
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*Spec, GetAbilitySystemComponent());
		}
	}
	else
	{
		UE_LOG(OWS, Verbose, TEXT("Owning Client: HandleProjectileDamage"));
		if (GetAbilitySystemComponent() != nullptr)
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*Spec, GetAbilitySystemComponent());
		}
	}
}

/*
void AOWSCharacterWithAbilities::HandleProjectileEffectApplicationPrediction(AOWSAdvancedProjectile* FakeProjectile, AActor* TargetActor)
{
	if (Role == ROLE_Authority)
	{
		UE_LOG(OWS, Error, TEXT("Server: HandleProjectileEffectApplicationPrediction Started"));

		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
		if (AbilitySystemInterface != nullptr)
		{
			UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

			if (TargetAbilitySystemComponent != nullptr)
			{
				FGameplayEffectSpec* Spec = FakeProjectile->DamageEffectOnHit.Data.Get();
				if (Spec)
				{
					AbilitySystem->ApplyGameplayEffectSpecToTarget(*FakeProjectile->DamageEffectOnHit.Data.Get(), TargetAbilitySystemComponent);
				}
			}
		}

		return;
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("Client: HandleProjectileEffectApplicationPrediction Started"));

		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
		if (AbilitySystemInterface != nullptr)
		{
			UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

			if (TargetAbilitySystemComponent != nullptr)
			{
				FGameplayEffectSpec* Spec = FakeProjectile->DamageEffectOnHit.Data.Get();
				if (Spec)
				{
					FScopedPredictionWindow PredictionWindow = FScopedPredictionWindow(AbilitySystem, true);

					UE_LOG(OWS, Error, TEXT("HandleProjectileEffectApplicationPrediction Apply GE"));

					AbilitySystem->ApplyGameplayEffectSpecToTarget(*FakeProjectile->DamageEffectOnHit.Data.Get(), TargetAbilitySystemComponent, PredictionWindow.ScopedPredictionKey);

					ReplicateProjectilePredictionKeyToServer(PredictionWindow.ScopedPredictionKey);
				}
			}
		}
	}
}
*/

/*
bool AOWSCharacterWithAbilities::ReplicateProjectilePredictionKeyToServer_Validate(FPredictionKey PredictionKey)
{
	return true;
}

void AOWSCharacterWithAbilities::ReplicateProjectilePredictionKeyToServer_Implementation(FPredictionKey PredictionKey)
{
	UE_LOG(OWS, Error, TEXT("Server: ReplicateProjectilePredictionKeyToServer_Implementation Started"));

	for (TActorIterator<AOWSAdvancedProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AOWSAdvancedProjectile* Projectile = *ActorItr;

		Projectile->SetPredictionKey(PredictionKey);

		continue;
	}
}
*/

void AOWSCharacterWithAbilities::LoadCharacterStats()
{
	UE_LOG(OWS, Verbose, TEXT("AOWSCharacterWithAbilities: LoadCharacterStats"));
	GetCharacterStats();
}

void AOWSCharacterWithAbilities::GetCharacterStats()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		GetWorld()->GetTimerManager().ClearTimer(OnGetCharacterStatsTimerHandle);
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacterWithAbilities::OnGetCharacterStatsResponseReceived);
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

void AOWSCharacterWithAbilities::OnGetCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			LoadCharacterStatsFromJSON(JsonObject);
			LoadAttributesFromJSON(JsonObject);

			LoadCustomCharacterStats();
			UpdateCharacterStatsAfterLoading();
			OnOWSAttributeInitalizationComplete();
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


void AOWSCharacterWithAbilities::LoadAttributesFromJSON(TSharedPtr<FJsonObject> JsonObject)
{
	static FProperty* ThirstProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Thirst));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ThirstProperty), JsonObject->GetNumberField("Thirst"));
	static FProperty* HungerProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Hunger));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HungerProperty), JsonObject->GetNumberField("Hunger"));

	OWSAttributes->SetMaxHealth(JsonObject->GetNumberField("MaxHealth"));
	OWSAttributes->SetHealth(JsonObject->GetNumberField("Health"));
	OWSAttributes->SetHealthRegenRate(JsonObject->GetNumberField("HealthRegenRate"));

	/*
	static UProperty* MaxHealthProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxHealth));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxHealthProperty), JsonObject->GetNumberField("MaxHealth"));
	static UProperty* HealthProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Health));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HealthProperty), JsonObject->GetNumberField("Health"));
	static UProperty* HealthRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, HealthRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HealthRegenRateProperty), JsonObject->GetNumberField("HealthRegenRate"));
	*/

	static FProperty* MaxManaProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxMana));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxManaProperty), JsonObject->GetNumberField("MaxMana"));
	static FProperty* ManaProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Mana));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ManaProperty), JsonObject->GetNumberField("Mana"));
	static FProperty* ManaRegenRateProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ManaRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ManaRegenRateProperty), JsonObject->GetNumberField("ManaRegenRate"));

	static FProperty* MaxEnergyProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxEnergy));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxEnergyProperty), JsonObject->GetNumberField("MaxEnergy"));
	static FProperty* EnergyProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Energy));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnergyProperty), JsonObject->GetNumberField("Energy"));
	static FProperty* EnergyRegenRateProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, EnergyRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnergyRegenRateProperty), JsonObject->GetNumberField("EnergyRegenRate"));

	static FProperty* MaxFatigueProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxFatigue));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxFatigueProperty), JsonObject->GetNumberField("MaxFatigue"));
	static FProperty* FatigueProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Fatigue));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FatigueProperty), JsonObject->GetNumberField("Fatigue"));
	static FProperty* FatigueRegenRateProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, FatigueRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FatigueRegenRateProperty), JsonObject->GetNumberField("FatigueRegenRate"));

	static FProperty* MaxStaminaProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxStamina));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxStaminaProperty), JsonObject->GetNumberField("MaxStamina"));
	static FProperty* StaminaProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Stamina));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StaminaProperty), JsonObject->GetNumberField("Stamina"));
	static FProperty* StaminaRegenRateProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, StaminaRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StaminaRegenRateProperty), JsonObject->GetNumberField("StaminaRegenRate"));

	static FProperty* MaxEnduranceProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxEndurance));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxEnduranceProperty), JsonObject->GetNumberField("MaxEndurance"));
	static FProperty* EnduranceProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Endurance));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnduranceProperty), JsonObject->GetNumberField("Endurance"));
	static FProperty* EnduranceRegenRateProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, EnduranceRegenRate));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnduranceRegenRateProperty), JsonObject->GetNumberField("EnduranceRegenRate"));

	static FProperty* StrengthProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Strength));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StrengthProperty), JsonObject->GetNumberField("Strength"));
	static FProperty* DexterityProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Dexterity));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DexterityProperty), JsonObject->GetNumberField("Dexterity"));
	static FProperty* ConstitutionProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Constitution));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ConstitutionProperty), JsonObject->GetNumberField("Constitution"));
	static FProperty* IntellectProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Intellect));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(IntellectProperty), JsonObject->GetNumberField("Intellect"));
	static FProperty* WisdomProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Wisdom));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(WisdomProperty), JsonObject->GetNumberField("Wisdom"));
	static FProperty* CharismaProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Charisma));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CharismaProperty), JsonObject->GetNumberField("Charisma"));
	static FProperty* AgilityProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Agility));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AgilityProperty), JsonObject->GetNumberField("Agility"));
	static FProperty* SpiritProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Spirit));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpiritProperty), JsonObject->GetNumberField("Spirit"));
	static FProperty* MagicProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Magic));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MagicProperty), JsonObject->GetNumberField("Magic"));
	static FProperty* FortitudeProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Fortitude));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FortitudeProperty), JsonObject->GetNumberField("Fortitude"));
	static FProperty* ReflexProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Reflex));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ReflexProperty), JsonObject->GetNumberField("Reflex"));
	static FProperty* WillpowerProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Willpower));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(WillpowerProperty), JsonObject->GetNumberField("Willpower"));

	static FProperty* BaseAttackProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BaseAttack));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BaseAttackProperty), JsonObject->GetNumberField("BaseAttack"));
	static FProperty* BaseAttackBonusProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BaseAttackBonus));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BaseAttackBonusProperty), JsonObject->GetNumberField("BaseAttackBonus"));
	static FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, AttackPower));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AttackPowerProperty), JsonObject->GetNumberField("AttackPower"));
	static FProperty* AttackSpeedProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, AttackSpeed));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AttackSpeedProperty), JsonObject->GetNumberField("AttackSpeed"));
	static FProperty* CritChanceProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, CritChance));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CritChanceProperty), JsonObject->GetNumberField("CritChance"));
	static FProperty* CritMultiplierProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, CritMultiplier));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CritMultiplierProperty), JsonObject->GetNumberField("CritMultiplier"));
	static FProperty* HasteProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Haste));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HasteProperty), JsonObject->GetNumberField("Haste"));
	static FProperty* SpellPowerProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, SpellPower));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpellPowerProperty), JsonObject->GetNumberField("SpellPower"));
	static FProperty* SpellPenetrationProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, SpellPenetration));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpellPenetrationProperty), JsonObject->GetNumberField("SpellPenetration"));
	static FProperty* DefenseProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Defense));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DefenseProperty), JsonObject->GetNumberField("Defense"));
	static FProperty* DodgeProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Dodge));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DodgeProperty), JsonObject->GetNumberField("Dodge"));
	static FProperty* ParryProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Parry));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ParryProperty), JsonObject->GetNumberField("Parry"));
	static FProperty* AvoidanceProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Avoidance));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AvoidanceProperty), JsonObject->GetNumberField("Avoidance"));
	static FProperty* VersatilityProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Versatility));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(VersatilityProperty), JsonObject->GetNumberField("Versatility"));
	static FProperty* MultishotProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Multishot));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MultishotProperty), JsonObject->GetNumberField("Multishot"));
	static FProperty* InitiativeProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Initiative));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(InitiativeProperty), JsonObject->GetNumberField("Initiative"));
	static FProperty* NaturalArmorProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, NaturalArmor));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(NaturalArmorProperty), JsonObject->GetNumberField("NaturalArmor"));
	static FProperty* PhysicalArmorProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, PhysicalArmor));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(PhysicalArmorProperty), JsonObject->GetNumberField("PhysicalArmor"));
	static FProperty* BonusArmorProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BonusArmor));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BonusArmorProperty), JsonObject->GetNumberField("BonusArmor"));
	static FProperty* ForceArmorProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ForceArmor));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ForceArmorProperty), JsonObject->GetNumberField("ForceArmor"));
	static FProperty* MagicArmorProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MagicArmor));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MagicArmorProperty), JsonObject->GetNumberField("MagicArmor"));
	static FProperty* ResistanceProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Resistance));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ResistanceProperty), JsonObject->GetNumberField("Resistance"));
	static FProperty* ReloadSpeedProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ReloadSpeed));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ReloadSpeedProperty), JsonObject->GetNumberField("ReloadSpeed"));
	static FProperty* RangeProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Range));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(RangeProperty), JsonObject->GetNumberField("Range"));
	static FProperty* SpeedProperty = FindFieldChecked<FProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Speed));
	GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpeedProperty), JsonObject->GetNumberField("Speed"));
}


void AOWSCharacterWithAbilities::UpdateCharacterStats()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacterWithAbilities::OnUpdateCharacterStatsResponseReceived);
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
			+ FString(TEXT("&MaxHealth=")) + FString::SanitizeFloat(OWSAttributes->MaxHealth.GetBaseValue())
			+ FString(TEXT("&Health=")) + FString::SanitizeFloat(OWSAttributes->Health.GetBaseValue())
			+ FString(TEXT("&HealthRegenRate=")) + FString::SanitizeFloat(OWSAttributes->HealthRegenRate.GetBaseValue())
			+ FString(TEXT("&MaxMana=")) + FString::SanitizeFloat(OWSAttributes->MaxMana.GetBaseValue())
			+ FString(TEXT("&Mana=")) + FString::SanitizeFloat(OWSAttributes->Mana.GetBaseValue())
			+ FString(TEXT("&ManaRegenRate=")) + FString::SanitizeFloat(OWSAttributes->ManaRegenRate.GetBaseValue())
			+ FString(TEXT("&MaxEnergy=")) + FString::SanitizeFloat(OWSAttributes->MaxEnergy.GetBaseValue())
			+ FString(TEXT("&Energy=")) + FString::SanitizeFloat(OWSAttributes->Energy.GetBaseValue())
			+ FString(TEXT("&EnergyRegenRate=")) + FString::SanitizeFloat(OWSAttributes->EnergyRegenRate.GetBaseValue())
			+ FString(TEXT("&MaxFatigue=")) + FString::SanitizeFloat(OWSAttributes->MaxFatigue.GetBaseValue())
			+ FString(TEXT("&Fatigue=")) + FString::SanitizeFloat(OWSAttributes->Fatigue.GetBaseValue())
			+ FString(TEXT("&FatigueRegenRate=")) + FString::SanitizeFloat(OWSAttributes->FatigueRegenRate.GetBaseValue())
			+ FString(TEXT("&MaxStamina=")) + FString::SanitizeFloat(OWSAttributes->MaxStamina.GetBaseValue())
			+ FString(TEXT("&Stamina=")) + FString::SanitizeFloat(OWSAttributes->Stamina.GetBaseValue())
			+ FString(TEXT("&StaminaRegenRate=")) + FString::SanitizeFloat(OWSAttributes->StaminaRegenRate.GetBaseValue())
			+ FString(TEXT("&MaxEndurance=")) + FString::SanitizeFloat(OWSAttributes->MaxEndurance.GetBaseValue())
			+ FString(TEXT("&Endurance=")) + FString::SanitizeFloat(OWSAttributes->Endurance.GetBaseValue())
			+ FString(TEXT("&EnduranceRegenRate=")) + FString::SanitizeFloat(OWSAttributes->EnduranceRegenRate.GetBaseValue())
			+ FString(TEXT("&Strength=")) + FString::SanitizeFloat(OWSAttributes->Strength.GetBaseValue())
			+ FString(TEXT("&Dexterity=")) + FString::SanitizeFloat(OWSAttributes->Dexterity.GetBaseValue())
			+ FString(TEXT("&Constitution=")) + FString::SanitizeFloat(OWSAttributes->Constitution.GetBaseValue())
			+ FString(TEXT("&Intellect=")) + FString::SanitizeFloat(OWSAttributes->Intellect.GetBaseValue())
			+ FString(TEXT("&Wisdom=")) + FString::SanitizeFloat(OWSAttributes->Wisdom.GetBaseValue())
			+ FString(TEXT("&Charisma=")) + FString::SanitizeFloat(OWSAttributes->Charisma.GetBaseValue())
			+ FString(TEXT("&Agility=")) + FString::SanitizeFloat(OWSAttributes->Agility.GetBaseValue())
			+ FString(TEXT("&Spirit=")) + FString::SanitizeFloat(OWSAttributes->Spirit.GetBaseValue())
			+ FString(TEXT("&Magic=")) + FString::SanitizeFloat(OWSAttributes->Magic.GetBaseValue())
			+ FString(TEXT("&Fortitude=")) + FString::SanitizeFloat(OWSAttributes->Fortitude.GetBaseValue())
			+ FString(TEXT("&Reflex=")) + FString::SanitizeFloat(OWSAttributes->Reflex.GetBaseValue())
			+ FString(TEXT("&Willpower=")) + FString::SanitizeFloat(OWSAttributes->Willpower.GetBaseValue())
			+ FString(TEXT("&BaseAttack=")) + FString::SanitizeFloat(OWSAttributes->BaseAttack.GetBaseValue())
			+ FString(TEXT("&BaseAttackBonus=")) + FString::SanitizeFloat(OWSAttributes->BaseAttackBonus.GetBaseValue())
			+ FString(TEXT("&AttackPower=")) + FString::SanitizeFloat(OWSAttributes->AttackPower.GetBaseValue())
			+ FString(TEXT("&AttackSpeed=")) + FString::SanitizeFloat(OWSAttributes->AttackSpeed.GetBaseValue())
			+ FString(TEXT("&CritChance=")) + FString::SanitizeFloat(OWSAttributes->CritChance.GetBaseValue())
			+ FString(TEXT("&CritMultiplier=")) + FString::SanitizeFloat(OWSAttributes->CritMultiplier.GetBaseValue())
			+ FString(TEXT("&Haste=")) + FString::SanitizeFloat(OWSAttributes->Haste.GetBaseValue())
			+ FString(TEXT("&SpellPower=")) + FString::SanitizeFloat(OWSAttributes->SpellPower.GetBaseValue())
			+ FString(TEXT("&SpellPenetration=")) + FString::SanitizeFloat(OWSAttributes->SpellPenetration.GetBaseValue())
			+ FString(TEXT("&Defense=")) + FString::SanitizeFloat(OWSAttributes->Defense.GetBaseValue())
			+ FString(TEXT("&Dodge=")) + FString::SanitizeFloat(OWSAttributes->Dodge.GetBaseValue())
			+ FString(TEXT("&Parry=")) + FString::SanitizeFloat(OWSAttributes->Parry.GetBaseValue())
			+ FString(TEXT("&Avoidance=")) + FString::SanitizeFloat(OWSAttributes->Avoidance.GetBaseValue())
			+ FString(TEXT("&Versatility=")) + FString::SanitizeFloat(OWSAttributes->Versatility.GetBaseValue())
			+ FString(TEXT("&Multishot=")) + FString::SanitizeFloat(OWSAttributes->Multishot.GetBaseValue())
			+ FString(TEXT("&Initiative=")) + FString::SanitizeFloat(OWSAttributes->Initiative.GetBaseValue())
			+ FString(TEXT("&NaturalArmor=")) + FString::SanitizeFloat(OWSAttributes->NaturalArmor.GetBaseValue())
			+ FString(TEXT("&PhysicalArmor=")) + FString::SanitizeFloat(OWSAttributes->PhysicalArmor.GetBaseValue())
			+ FString(TEXT("&BonusArmor=")) + FString::SanitizeFloat(OWSAttributes->BonusArmor.GetBaseValue())
			+ FString(TEXT("&ForceArmor=")) + FString::SanitizeFloat(OWSAttributes->ForceArmor.GetBaseValue())
			+ FString(TEXT("&MagicArmor=")) + FString::SanitizeFloat(OWSAttributes->MagicArmor.GetBaseValue())
			+ FString(TEXT("&Resistance=")) + FString::SanitizeFloat(OWSAttributes->Resistance.GetBaseValue())
			+ FString(TEXT("&ReloadSpeed=")) + FString::SanitizeFloat(OWSAttributes->ReloadSpeed.GetBaseValue())
			+ FString(TEXT("&Range=")) + FString::SanitizeFloat(OWSAttributes->Range.GetBaseValue())
			+ FString(TEXT("&Speed=")) + FString::SanitizeFloat(OWSAttributes->Speed.GetBaseValue())
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

void AOWSCharacterWithAbilities::OnUpdateCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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


/*
TArray<FGameplayTag> AOWSCharacterWithAbilities::GetCombatHasIconTags()
{
	TArray<FGameplayTag> CombatHasIconTags;

	static FGameplayTagContainer OwnerTags;
	OwnerTags.Reset();
	GetAbilitySystemComponent()->GetOwnedGameplayTags(OwnerTags);

	OwnerTags.GetGameplayTagArray(CombatHasIconTags);

	return CombatHasIconTags;
}
*/
