// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/GameplayAbility/AttributeSet/ZSVehicleAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UZSVehicleAttributeSet::UZSVehicleAttributeSet()
{
	
}

void UZSVehicleAttributeSet::OnRep_HealthBody(const FGameplayAttributeData& OldHealthBody)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, HealthBody, OldHealthBody);
}

void UZSVehicleAttributeSet::OnRep_MaxHealthBody(const FGameplayAttributeData& OldMaxHealthBody)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, MaxHealthBody, OldMaxHealthBody);
}

void UZSVehicleAttributeSet::OnRep_HealthEngine(const FGameplayAttributeData& OldHealthEngine)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, HealthEngine, OldHealthEngine);
}

void UZSVehicleAttributeSet::OnRep_MaxHealthEngine(const FGameplayAttributeData& OldMaxHealthEngine)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, MaxHealthEngine, OldMaxHealthEngine);
}

void UZSVehicleAttributeSet::OnRep_GasTank(const FGameplayAttributeData& OldGasTank)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, GasTank, OldGasTank);
}

void UZSVehicleAttributeSet::OnRep_MaxGasTank(const FGameplayAttributeData& OldMaxGasTank)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, MaxGasTank, OldMaxGasTank);
}

void UZSVehicleAttributeSet::OnRep_StopRearLight(const FGameplayAttributeData& OldStopRearLight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, StopRearLight, OldStopRearLight);
}

void UZSVehicleAttributeSet::OnRep_MaxStopRearLight(const FGameplayAttributeData& OldMaxStopRearLight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, MaxStopRearLight, OldMaxStopRearLight);
}


void UZSVehicleAttributeSet::OnRep_RearLight(const FGameplayAttributeData& OldRearLight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, RearLight, OldRearLight);
}

void UZSVehicleAttributeSet::OnRep_MaxRearLight(const FGameplayAttributeData& OldMaxRearLight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZSVehicleAttributeSet, MaxRearLight, OldMaxRearLight);
}

void UZSVehicleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& NewData)
{
	Super::PostGameplayEffectExecute(NewData);

	if(NewData.EvaluatedData.Attribute == GetHealthBodyAttribute())
	{
		SetMaxHealthBody(FMath::Clamp<float>(GetHealthBody(), 0.0, GetMaxHealthBody() ));
	}
	if(NewData.EvaluatedData.Attribute == GetHealthEngineAttribute())
	{
		SetMaxHealthEngine(FMath::Clamp<float>(GetHealthEngine(), 0.0, GetMaxHealthEngine()));
	}
	if(NewData.EvaluatedData.Attribute == GetGasTankAttribute())
	{
		SetMaxGasTank(FMath::Clamp<float>(GetGasTank(), 0.0, GetMaxGasTank()));
	}
	if(NewData.EvaluatedData.Attribute == GetStopRearLightAttribute())
	{
		SetMaxStopRearLight(FMath::Clamp<float>(GetStopRearLight(), 0.0, GetMaxStopRearLight()));
	}
	if(NewData.EvaluatedData.Attribute == GetRearLightAttribute())
	{
		SetMaxStopRearLight(FMath::Clamp<float>(GetRearLight(), 0.0, GetMaxRearLight()));
	}
}

void UZSVehicleAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, HealthBody, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, MaxHealthBody, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, HealthEngine, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, MaxHealthEngine, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, GasTank, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, MaxGasTank, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, StopRearLight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, MaxStopRearLight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, RearLight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZSVehicleAttributeSet, MaxRearLight, COND_None, REPNOTIFY_Always);
}
