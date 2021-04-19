// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSpaceGameInstance.h"

#include "../Components/ManageWidgetsResolution.h"
#include <Kismet/KismetSystemLibrary.h>

#include "WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"

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

class UManageWidgetsResolution* UZSpaceGameInstance::GetManageWidgetsResolution() const
{
	return ManageWidgetsResolution;
}

void UZSpaceGameInstance::CreateWidgetLoadingManagerObject()
{
	if( GetWorld() && GetWorld()->IsClient() )
	{
		checkf(nullptr != WidgetLoadingManagerObjectSubClass, TEXT("The WidgetLoadingManagerObjectSubClass isn't set, Please set"));
		if(WidgetLoadingManagerObjectSubClass)
		{
			WidgetLoadingManagerObject = NewObject<UWidgetLoadingManagerObject>(this, WidgetLoadingManagerObjectSubClass);
		}
	}
}

void UZSpaceGameInstance::Init()
{
	Super::Init();
	CreateWidgetLoadingManagerObject();

	
	if (UKismetSystemLibrary::IsStandalone(this) || !UKismetSystemLibrary::IsServer(this))
	{
		InitManageWidgetsResolution();
	}
}

UWidgetLoadingManagerObject* UZSpaceGameInstance::GetWidgetLoadingManagerObject() const
{
	return WidgetLoadingManagerObject;
}

