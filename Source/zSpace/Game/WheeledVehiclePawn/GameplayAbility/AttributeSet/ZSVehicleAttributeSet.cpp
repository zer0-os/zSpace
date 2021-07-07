// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/GameplayAbility/AttributeSet/ZSVehicleAttributeSet.h"

#include "GameplayEffectExtension.h"

UZSVehicleAttributeSet::UZSVehicleAttributeSet()
{
	
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
}
