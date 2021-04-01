// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSpaceGameInstance.h"

#include "../Components/ManageWidgetsResolution.h"
#include <Kismet/KismetSystemLibrary.h>

UZSpaceGameInstance::UZSpaceGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZSpaceGameInstance::InitManageWidgetsResolution()
{
	check(ManageWidgetsResolutionSubClass);

	if (ManageWidgetsResolutionSubClass)
	{
		ManageWidgetsResolution = NewObject<UManageWidgetsResolution>(
			this->GetOuter(), ManageWidgetsResolutionSubClass
		);
	}
}

void UZSpaceGameInstance::Init()
{
	Super::Init();

	if (UKismetSystemLibrary::IsStandalone(this) || !UKismetSystemLibrary::IsServer(this))
	{
		InitManageWidgetsResolution();
	}
}
