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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_HealthBody, Category="BaseAttributes")
	FGameplayAttributeData HealthBody;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, HealthBody);

	UFUNCTION()
	void OnRep_HealthBody(const FGameplayAttributeData & OldHealthBody);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxHealthBody, Category="BaseAttributes")
	FGameplayAttributeData MaxHealthBody;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxHealthBody);

	UFUNCTION()
	void OnRep_MaxHealthBody(const FGameplayAttributeData & OldMaxHealthBody);
	
	// Attributes for HealthBody
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_HealthEngine, Category="BaseAttributes")
	FGameplayAttributeData HealthEngine;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, HealthEngine);

	UFUNCTION()
	void OnRep_HealthEngine(const FGameplayAttributeData & OldHealthEngine);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,ReplicatedUsing=OnRep_MaxHealthEngine, Category="BaseAttributes")
	FGameplayAttributeData MaxHealthEngine;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxHealthEngine);
	
	UFUNCTION()
	void OnRep_MaxHealthEngine(const FGameplayAttributeData & OldMaxHealthEngine);
	
	// Attributes for Gas Tank
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_GasTank, Category="BaseAttributes")
	FGameplayAttributeData GasTank;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, GasTank);
	
	UFUNCTION()
	void OnRep_GasTank(const FGameplayAttributeData & OldGasTank);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxGasTank, Category="BaseAttributes")
	FGameplayAttributeData MaxGasTank;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxGasTank);
	
	UFUNCTION()
	void OnRep_MaxGasTank(const FGameplayAttributeData & OldMaxGasTank);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_StopRearLight , Category="BaseAttributes")
	FGameplayAttributeData StopRearLight;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, StopRearLight);
	
	UFUNCTION()
	void OnRep_StopRearLight(const FGameplayAttributeData & OldStopRearLight);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxStopRearLight, Category="BaseAttributes")
	FGameplayAttributeData MaxStopRearLight;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxStopRearLight);
	
	UFUNCTION()
	void OnRep_MaxStopRearLight(const FGameplayAttributeData & OldMaxStopRearLight);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_RearLight, Category="BaseAttributes")
	FGameplayAttributeData RearLight;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, RearLight);
	
	UFUNCTION()
	void OnRep_RearLight(const FGameplayAttributeData & OldRearLight);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxRearLight, Category="BaseAttributes")
	FGameplayAttributeData MaxRearLight;
	ATTRIBUTE_ACCESSORS(UZSVehicleAttributeSet, MaxRearLight);
	
	UFUNCTION()
	void OnRep_MaxRearLight(const FGameplayAttributeData & OldMaxRearLight);

		

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& NewData) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};





