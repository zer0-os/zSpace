// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/LoginUserWidgetBase.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSLoginPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/EditableTextBox.h"
#include "../Game/ZSpaceGameInstance.h"
#include "Components/Button.h"
#include "../Types/UITypes.h"
#include "zSpace/zSpace.h"
#include "ZSpaceButton.h"
#include "MediaPlayer.h"
#include "MediaSource.h"


void ULoginUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(MediaPlayerPreviewCard) && IsValid(MediaSourcePreviewCard))
	{
		MediaPlayerPreviewCard->OpenSource(MediaSourcePreviewCard);
	}
	
	if (IsValid(txtEmail) && IsValid(txtPassword))
	{
		if (!txtEmail->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtEmail->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
		if (!txtPassword->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtPassword->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
	}

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		if (!PlayerController->OnEscButtonPressed.IsAlreadyBound(this, &ULoginUserWidgetBase::ToPreviousMenu))
		{
			PlayerController->OnEscButtonPressed.AddDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
		}
	}

	if (IsValid(LoginEnter))
	{
		LoginEnter->ButtonWidget->OnClicked.AddUniqueDynamic(this, &ULoginUserWidgetBase::OnClickedLoginEnter);
	}
	if (IsValid(BtnPreviousMenu))
	{
		BtnPreviousMenu->OnClicked.AddUniqueDynamic(this, &ULoginUserWidgetBase::OnClickedBtnPreviousMenu);
	}

	OpacityTimelineFloat.BindUFunction(this, FName("UpdatingOpacityTimeLine"));
	OpacityTimeLine.AddInterpFloat(OpacityCurveFloat, OpacityTimelineFloat);
}

void ULoginUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
	
	// UpdatingOpacityTimeLine()
	OpacityTimeLine.PlayFromStart();
}

void ULoginUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
	}
}

void ULoginUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OpacityTimeLine.TickTimeline(InDeltaTime);
}

void ULoginUserWidgetBase::Login()
{
	if (!IsValid(txtEmail) && !IsValid(txtPassword)) return;

	const FString Email = txtEmail->GetText().ToString();
	const FString Password = txtPassword->GetText().ToString();

	LoginAndCreateSession(Email, Password);
	UnBindToPreviousMenuEvent();
}

void ULoginUserWidgetBase::OnClickedLoginEnter()
{
	Login();
}

void ULoginUserWidgetBase::OnClickedBtnPreviousMenu()
{
	ToPreviousMenu();
	RemoveFromParent();
}

EWidgetType ULoginUserWidgetBase::GetWidgetType_Implementation()
{
	return EWidgetType::Login;
}

void ULoginUserWidgetBase::UpdatingOpacityTimeLine(float Value)
{
	SetRenderOpacity(Value);
}

void ULoginUserWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		Login();
		UnBindToPreviousMenuEvent();
	}
}

void ULoginUserWidgetBase::UnBindToPreviousMenuEvent()
{
	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
	}
}

void ULoginUserWidgetBase::ToPreviousMenu()
{
	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (!IsValid(GameInstance)) return;

	UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	if (!IsValid(ManageWidgetsResolution)) return;

	const EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UResolutionAndWidgetDataAsset* WidgetDataAsset = IUIResolutionInterface::Execute_GetToPreviousMenuDataAsset(this);
	const TSubclassOf<UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &ULoginUserWidgetBase::ToPreviousMenu);
	}

	RemoveFromParent();
}

void ULoginUserWidgetBase::LoginSuccess(const FString& UserSessionGUID)
{
	AZSLoginPlayerController* PC = GetOwningPlayer<AZSLoginPlayerController>();
	if (IsValid(PC))
	{
		PC->SetUserSessionGUID(UserSessionGUID);
		UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
		if (IsValid(GameInstance))
		{
			GameInstance->UserSessionGUID = UserSessionGUID;
			UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
			if (IsValid(ManageWidgetsResolution))
			{
				UResolutionAndWidgetDataAsset* Asset = ManageWidgetsResolution->GetWidgetDataAssetByWidgetType(EWidgetType::SelectCharacter);
				if (IsValid(Asset))
				{
					const TSubclassOf<UUserWidget>& Widget = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, Asset);
					const EResolution& Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

					UUserWidget* CreatedWidget = nullptr;
					ManageWidgetsResolution->CreateWidgetAndAddViewport(PC, Widget, Resolution, CreatedWidget);
					if (IsValid(CreatedWidget))
					{
						PC->GetAllCharacters(PC->GetUserSessionGUID());
						RemoveFromParent();
					}
				}
			}
		}
	}
}
