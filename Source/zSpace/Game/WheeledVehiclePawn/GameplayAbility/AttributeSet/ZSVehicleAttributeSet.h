// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ZSVehicleAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSVehicleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UZSVehicleAttributeSet();

	// Attributes for HealthBody
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData HealthBody;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, HealthBody);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData MaxHealthBody;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxHealthBody);
	
	// Attributes for HealthBody
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData HealthEngine;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, HealthEngine);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData MaxHealthEngine;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxHealthEngine);
	
	// Attributes for Gas Tank
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData GasTank;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, GasTank);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseAttributes")
	FGameplayAttributeData MaxGasTank;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxGasTank);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& NewData) override;
};
