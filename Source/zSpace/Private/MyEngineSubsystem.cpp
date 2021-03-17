// Copyright 2018 Sabre Dart Studios


#include "MyEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UMyEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}