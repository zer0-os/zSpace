// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/RegisterUserWidgetBase.h"

#include "../BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "../PlayerController/ZSLoginPlayerController.h"
#include "../Components/ManageWidgetsResolution.h"
#include "Components/EditableTextBox.h"
#include "../Game/ZSpaceGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/UI/ZSpaceButton.h"
#include "LoginUserWidgetBase.h"
#include "Components/Button.h"
#include "../Types/UITypes.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "zSpace/zSpace.h"

void URegisterUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (IsValid(MediaPlayerPreviewCard) && IsValid(MediaSourcePreviewCard))
	{
		MediaPlayerPreviewCard->OpenSource(MediaSourcePreviewCard);
	}

	if (IsValid(BtnRegister))
	{
		UButton* Button = BtnRegister->ButtonWidget;
		if (IsValid(Button))
		{
			if (!Button->OnClicked.IsAlreadyBound(this, &URegisterUserWidgetBase::BtnRegisterOnClicked))
			{
				Button->OnClicked.AddDynamic(this, &URegisterUserWidgetBase::BtnRegisterOnClicked);
			}
		}
	}
	if (IsValid(BtnCancel))
	{
		if (!BtnCancel->OnClicked.IsAlreadyBound(this, &URegisterUserWidgetBase::BtnCancelOnClicked))
		{
			BtnCancel->OnClicked.AddDynamic(this, &URegisterUserWidgetBase::BtnCancelOnClicked);
		}
	}

	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	}

	BindOnTextCommittedEvent();

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		if (!PlayerController->OnEscButtonPressed.IsAlreadyBound(this, &URegisterUserWidgetBase::ToPreviousMenu))
		{
			PlayerController->OnEscButtonPressed.AddDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
		}
	}
	
	OpacityTimelineFloat.BindUFunction(this, FName("UpdatingOpacityTimeLine"));
	OpacityTimeLine.AddInterpFloat(OpacityCurveFloat, OpacityTimelineFloat);
}

void URegisterUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
	
	// UpdatingOpacityTimeLine()
	OpacityTimeLine.PlayFromStart();
}

void URegisterUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OpacityTimeLine.TickTimeline(InDeltaTime);
}

void URegisterUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
	}	
}

void URegisterUserWidgetBase::BtnRegisterOnClicked()
{
	const FString StringFirstName = FirstName->GetText().ToString();
	const FString StringLastName = LastName->GetText().ToString();
	const FString StringEmail = Email->GetText().ToString();
	const FString StringPassword = Password->GetText().ToString();

	Register(StringEmail, StringPassword, StringFirstName, StringLastName);
}

void URegisterUserWidgetBase::BtnCancelOnClicked()
{
	if (!IsValid(ManageWidgetsResolution)) return;
	
	RemoveFromParent();

	UResolutionAndWidgetDataAsset* WidgetDataAsset = ManageWidgetsResolution->GetWidgetDataAssetByWidgetType(EWidgetType::PreLogin);

	const TSubclassOf<class UUserWidget>& WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	const EResolution& Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UUserWidget* Widget = nullptr;
	
	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
}

void URegisterUserWidgetBase::OnSuccessRegister()
{
	if (!IsValid(ManageWidgetsResolution)) return;

	UResolutionAndWidgetDataAsset* WidgetDataAsset = ManageWidgetsResolution->GetWidgetDataAssetByWidgetType(EWidgetType::Login);

	const TSubclassOf<class UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	const EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);

	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);
	if (IsValid(Widget))
	{
		ULoginUserWidgetBase* LoginWidget = Cast<ULoginUserWidgetBase>(Widget);
		if (IsValid(LoginWidget))
		{
			LoginWidget->txtEmail->SetText(Email->GetText());
		}
	}

	RemoveFromParent();
}

void URegisterUserWidgetBase::ToPreviousMenu()
{
	const EResolution Resolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
	UResolutionAndWidgetDataAsset* WidgetDataAsset = IUIResolutionInterface::Execute_GetToPreviousMenuDataAsset(this);
	const TSubclassOf<UUserWidget> WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, WidgetDataAsset);
	UUserWidget* Widget = nullptr;

	ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, Resolution, Widget);

	AZSLoginPlayerController* PlayerController = Cast<AZSLoginPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->OnEscButtonPressed.RemoveDynamic(this, &URegisterUserWidgetBase::ToPreviousMenu);
	}

	RemoveFromParent();
}

void URegisterUserWidgetBase::UpdatingOpacityTimeLine(float Value)
{
	SetRenderOpacity(Value);
}

void URegisterUserWidgetBase::BindOnTextCommittedEvent()
{
	auto Bind = [this] (UEditableTextBox* EditableTextBox) -> void
	{
		if (IsValid(EditableTextBox) && !EditableTextBox->OnTextCommitted.IsAlreadyBound(this, &URegisterUserWidgetBase::OnTextCommitted))
		{
			EditableTextBox->OnTextCommitted.AddDynamic(this, &URegisterUserWidgetBase::OnTextCommitted);
		}
	};

 	Bind(FirstName);
 	Bind(LastName);
 	Bind(Email);
 	Bind(Password);
}

void URegisterUserWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		BtnRegisterOnClicked();
	}
}

EWidgetType URegisterUserWidgetBase::GetWidgetType_Implementation()
{
	return EWidgetType::Register;
}

