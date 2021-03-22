// Copyright 2018 Sabre Dart Studios


#include "ZPEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UZPEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}