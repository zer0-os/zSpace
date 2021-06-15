// Copyright 2020 Sabre Dart Studios


#include "VRLoginAndRegWidget.h"

#include "VRLoginOrRegUserWidget.h"
#include "VRLoginUserWidget.h"
#include "VRRegUserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "zSpace/PlayerController/ZSLoginPlayerController.h"

UVRLoginAndRegWidget::UVRLoginAndRegWidget(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsReg = true;	
}

void UVRLoginAndRegWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVRLoginAndRegWidget::NativePreConstruct()
{
	if(IsValid(WidgetSwitcher))
	{
		if(IsValid(VRLoginUserWidget))
		{
			VRLoginUserWidget->VRLoginAndRegWidget = this;
		}
		if(IsValid(VRRegUserWidget))
		{
			VRRegUserWidget->VRLoginAndRegWidget = this;
		}
		if(IsValid(VRLoginOrRegUserWidget))
		{
			VRLoginOrRegUserWidget->VRLoginAndRegWidget = this;			
		}
		
	}
	Super::NativePreConstruct();
}

void UVRLoginAndRegWidget::Login(FString NewLogin, FString NewPassword)
{
	// TODO need to check 
	LoginAndCreateSession(NewLogin, NewPassword);
}

void UVRLoginAndRegWidget::Reg(FString NewEmail, FString NewPassword, FString NewFirstName, FString NewLastName)
{
	// TODO need to check
	// Store Email and password to use after register 
	TempLogin = NewEmail;
	TempPassword = NewPassword;
	Register(NewEmail, NewPassword, NewFirstName, NewLastName);
}

void UVRLoginAndRegWidget::LoginSuccess(FString NewUserSessionGUID)
{
	UZSpaceGameInstance * ZSpaceGameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(IsValid(ZSpaceGameInstance))
	{
		ZSpaceGameInstance->UserSessionGUID = NewUserSessionGUID;
		if(bIsReg)
		{
			AZSLoginPlayerController *  PC = Cast<AZSLoginPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if(IsValid(PC))
			{
				const FString UserName = AZSLoginPlayerController::GetRandomString(9);
				PC->CreateCharacter(ZSpaceGameInstance->UserSessionGUID, UserName, AZSLoginPlayerController::ClassName);
			}
		}
	}
}

void UVRLoginAndRegWidget::RegSuccess()
{
	bIsReg = true;
	Login(TempLogin, TempPassword);
}

void UVRLoginAndRegWidget::SwitchLoginWidget()
{
	checkf(nullptr != WidgetSwitcher, TEXT("The WidgetSwitcher is nullptr."));
	if(IsValid(WidgetSwitcher))
	{
		checkf(nullptr != WidgetSwitcher, TEXT("The WidgetSwitcher is nullptr.") );
		if(IsValid(WidgetSwitcher))
		{
			WidgetSwitcher->SetActiveWidget(VRLoginUserWidget);
		}
	}
}

void UVRLoginAndRegWidget::SwitchRegWidget()
{
	checkf(nullptr != WidgetSwitcher, TEXT("The WidgetSwitcher is nullptr."));
	if(IsValid(WidgetSwitcher))
	{
		checkf(nullptr != VRRegUserWidget, TEXT("The VRRegUserWidget is nullptr."));
		if(IsValid(VRRegUserWidget))
		{
			WidgetSwitcher->SetActiveWidget(VRRegUserWidget);
		}
	}
	
}

void UVRLoginAndRegWidget::SwitchLoginOrRegUserWidget()
{
		
	checkf(nullptr != WidgetSwitcher, TEXT("The WidgetSwitcher is nullptr."));
	if(IsValid(WidgetSwitcher))
	{
		checkf(nullptr != VRLoginOrRegUserWidget, TEXT("The VRLoginOrRegUserWidget is nullptr."));
		if(IsValid(VRLoginOrRegUserWidget))
		{
			WidgetSwitcher->SetActiveWidget(VRLoginOrRegUserWidget);
		}
	}
	
}

