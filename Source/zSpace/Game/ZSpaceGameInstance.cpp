// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/ZSpaceGameInstance.h"


#include "../Components/ManageWidgetsResolution.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "SoundManager/SoundManager.h"
#include "ZSGamePlayerController/ZSGamePlayerController.h"
#include "zSpace/zSpace.h"

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
	if (IsValid(SoundManagerClass))
	{	
		SoundManagerRef = NewObject<USoundManager>(this, SoundManagerClass);
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
			if(WidgetLoadingManagerObject )
			{
				WidgetLoadingManagerObject->SetZSpaceGameInstance(this);
			}
		}
	}
}

void UZSpaceGameInstance::Init()
{
	Super::Init();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UZSpaceGameInstance::BeginLoadingLevel);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UZSpaceGameInstance::EndLoadingLevel);
	
	CreateWidgetLoadingManagerObject();
	
	if (UKismetSystemLibrary::IsStandalone(this) || !UKismetSystemLibrary::IsServer(this))
	{
		InitManageWidgetsResolution();
	}
	
}

void UZSpaceGameInstance::BeginLoadingLevel(const FString& MapName)
{
	const FString V = FString::Printf(TEXT("******************** The %s Lave is loaded ****************"), *MapName);
	//UKismetSystemLibrary::PrintString(this, V, true, true, FLinearColor::Red, 111);
	FTimerHandle L_TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(L_TimerHandler, [&]()
	{
		if(WidgetLoadingManagerObject)
		{
			const TSubclassOf<UUserWidget> L_SecondLoadingWidget = WidgetLoadingManagerObject->SecondLoadingWidget.LoadSynchronous();
			WidgetLoadingManagerObject->ShowLoadingWidget(L_SecondLoadingWidget , 99);
		}
	}, 0.1, false);
}

void UZSpaceGameInstance::EndLoadingLevel(UWorld* NewWorld)
{
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndLoadingLevel, [&]()
	{
		const FString V = FString::Printf(TEXT("******************** EndLoadingLevel ****************"));
		//UKismetSystemLibrary::PrintString(this, V, true, true, FLinearColor::Red, 111);
		// Check level is WorldComposition ?
		if(GetWorld() && GetWorld()->WorldComposition)
		{
			AZSGamePlayerController * PC = Cast<AZSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if(nullptr != PC)
			{
				//UKismetSystemLibrary::PrintString(this, "----------------- The PC is Valid -----------", true, true, FLinearColor::Red, 111);
				PC->OnNotifyGetAllUserCharacters.AddUniqueDynamic(this, &UZSpaceGameInstance::NotifyGetAllUserCharacters);
				PC->GetAllCharacters(UserSessionGUID); // call back function NotifyGetAllUserCharacters
			}
		}
		else // When level is not WorldComposition 
		{
			if(nullptr != WidgetLoadingManagerObject)
			{
				WidgetLoadingManagerObject->HideLoadingWidget();	
			}
		}

		if (IsValid(SoundManagerRef))
		{
			SoundManagerRef->SetSoundVolumeBySoundClass(FName("Gameplay"), 1);
			SoundManagerRef->SetSoundVolumeBySoundClass(FName("Ambient"), 1);
		}

	}, 1.2, false);


}

UWidgetLoadingManagerObject* UZSpaceGameInstance::GetWidgetLoadingManagerObject() const
{
	return WidgetLoadingManagerObject;
}

class USoundManager* UZSpaceGameInstance::GetSoundManagerObject() const
{
	return SoundManagerRef;
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

void UZSpaceGameInstance::SetCurrentSelectedCharacterIndex(const int32& NewCurrentSelectedCharacterIndex)
{
	CurrentSelectedCharacterIndex = NewCurrentSelectedCharacterIndex;
}

void UZSpaceGameInstance::NotifyGetAllUserCharacters(const TArray<FUserCharacter>& NewUserCharacter)
{
	UserCharacter = NewUserCharacter;
	if(-1 != CurrentSelectedCharacterIndex)
	{
		if(UserCharacter.IsValidIndex(CurrentSelectedCharacterIndex))
		{
			const FUserCharacter L_UserCharacter = UserCharacter[CurrentSelectedCharacterIndex];
			FString L_ZoneName = L_UserCharacter.ZoneName;
			ULevelStreaming * L_LevelStreaming  = UGameplayStatics::GetStreamingLevel(GetWorld(), FName(L_ZoneName));
			if(IsValid(L_LevelStreaming) && false == L_LevelStreaming->IsLevelLoaded())
			{
				const FString V = FString::Printf(TEXT("******************** +++++++++++++++++ ****************"));
				//UKismetSystemLibrary::PrintString(this, V, true, true, FLinearColor::Red, 111);
				L_LevelStreaming->OnLevelLoaded.AddUniqueDynamic(this, &UZSpaceGameInstance::StreamingLevelLoaded);	
			}
			else if(L_LevelStreaming && L_LevelStreaming->IsLevelLoaded())
			{
				const FString V = FString::Printf(TEXT("******************** **************** ****************"));
				//UKismetSystemLibrary::PrintString(this, V, true, true, FLinearColor::Red, 111);
				StreamingLevelLoaded();
			}
			else if(GetWorld() && nullptr == GetWorld()->WorldComposition)
			{
				//UKismetSystemLibrary::PrintString(this, TEXT("*************** //// *****************"), true, true, FLinearColor::Red, 111);
				StreamingLevelLoaded();
			}
			else if(nullptr == L_LevelStreaming)
			{
				const FString V = FString::Printf(TEXT("******************** ----------------------- ****************"));
				//UKismetSystemLibrary::PrintString(this, V, true, true, FLinearColor::Red, 111);
				UE_LOG(LogTemp, Warning, TEXT("The L_LevelStreming is nullptr."));
				UE_LOG(LogTemp, Warning, TEXT("********** Level Name %s *****************"), *L_ZoneName);
			}
		}
	}
}

void UZSpaceGameInstance::StreamingLevelLoaded()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Starting HideLoading."));
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleLoadLevel, [&]()
		{
			if(WidgetLoadingManagerObject)
			{
				WidgetLoadingManagerObject->HideLoadingWidget();	
			}
		}, 3,  false);
	}
}


