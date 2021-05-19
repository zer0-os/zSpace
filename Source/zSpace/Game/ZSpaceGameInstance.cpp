// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSpaceGameInstance.h"

#include "../Components/ManageWidgetsResolution.h"
#include <Kismet/KismetSystemLibrary.h>

#include "Blueprint/UserWidget.h"
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

void UZSpaceGameInstance::ShowCreateWalletWidget()
{
	if(nullptr == CreateWalletWidget)
	{
		const TSubclassOf<UUserWidget>  L_UserWidgetWallet =  CreateWalletWidgetSoftClassPtr.LoadSynchronous();
		if(L_UserWidgetWallet)
		{
			CreateWalletWidget = CreateWidget<UUserWidget>(this, L_UserWidgetWallet);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The CrateWalletWidget is not set"));
		}
	}
	if(CreateWalletWidget)
	{
		if(false == CreateWalletWidget->IsInViewport())
		{
			const int32 L_ZOrder = 10;
			CreateWalletWidget->AddToViewport(L_ZOrder);
		}
	}
}

void UZSpaceGameInstance::HideCreateWalletWidget()
{
	if(nullptr != CreateWalletWidget)
	{
		CreateWalletWidget->RemoveFromViewport();
		CreateWalletWidget = nullptr;
	}
}

void UZSpaceGameInstance::ShowLoadRemoteWalletWidget()
{
	if(nullptr == LoadRemoteWalletWidget)
	{
		const TSubclassOf<UUserWidget>  L_UserWidgetWallet =  LoadRemoteWalletWidgetSoftClassPtr.LoadSynchronous();
		if(L_UserWidgetWallet)
		{
			LoadRemoteWalletWidget = CreateWidget<UUserWidget>(this, L_UserWidgetWallet);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The LoadRemoteWalletWidget is not set"));
		}
	}
	if(LoadRemoteWalletWidget)
	{
		if(false == LoadRemoteWalletWidget->IsInViewport())
		{
			const int32 L_ZOrder = 10;
			LoadRemoteWalletWidget->AddToViewport(L_ZOrder);
		}
	}
	
}

void UZSpaceGameInstance::HideLoadRemoteWalletWidget()
{
	if(nullptr != LoadRemoteWalletWidget)
	{
		LoadRemoteWalletWidget->RemoveFromViewport();
		LoadRemoteWalletWidget = nullptr;
	}
}

void UZSpaceGameInstance::ShowLoadWalletWithMnemonicWidget()
{
	if(nullptr == LoadWalletWithMnemonicWidget)
	{
		const TSubclassOf<UUserWidget>  L_UserWidgetWallet =  LoadWalletWithMnemonicWidgetSoftClassPtr.LoadSynchronous();
		if(L_UserWidgetWallet)
		{
			LoadWalletWithMnemonicWidget = CreateWidget<UUserWidget>(this, L_UserWidgetWallet);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The LoadWalletWithMnemonicWidget is not set"));
		}
	}
	if(LoadWalletWithMnemonicWidget)
	{
		if(false == LoadWalletWithMnemonicWidget->IsInViewport())
		{
			const int32 L_ZOrder = 10;
			LoadWalletWithMnemonicWidget->AddToViewport(L_ZOrder);
		}
	}
}

void UZSpaceGameInstance::HideLoadWalletWithMnemonicWidget()
{
	if(nullptr != LoadWalletWithMnemonicWidget)
	{
		LoadWalletWithMnemonicWidget->RemoveFromViewport();
		LoadWalletWithMnemonicWidget = nullptr;
	}
}


