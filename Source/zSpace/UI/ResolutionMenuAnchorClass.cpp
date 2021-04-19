// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/ResolutionMenuAnchorClass.h"


#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "ZSCustomButton.h"
#include "zSpace/Components/ManageWidgetsResolution.h"
#include "zSpace/Game/ZSpaceGameInstance.h"

void UResolutionMenuAnchorClass::NativePreConstruct()
{
	Super::NativePreConstruct();

	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
	}

	CreateDefaultButtons();
}

void UResolutionMenuAnchorClass::OnSelectResolution(UZSCustomButton* Button)
{
	if (IsValid(Button))
	{
		const FString Resolution = UUIBlueprintFunctionLibrary::GetResolutionByEnum(this, Button->Resolution, EWindowMode::Type::Windowed);
		UKismetSystemLibrary::PrintString(this, Resolution);
		const FString Command = "r.SetRes " + Resolution;
		

		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command, GetOwningPlayer());
		if (IsValid(ResolutionAndWidgetDataAsset) && IsValid(ManageWidgetsResolution))
		{
			const auto WidgetSubClass = UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(this, ResolutionAndWidgetDataAsset);
			const EResolution ResolutionEnum = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
			UUserWidget* NewWidget = nullptr;

			// UKismetSystemLibrary::PrintString(this, GetNameSafe(ResolutionAndWidgetDataAsset));
			
 			ManageWidgetsResolution->CreateWidgetAndAddViewport(GetOwningPlayer(), WidgetSubClass, ResolutionEnum, NewWidget);
			UKismetSystemLibrary::PrintString(this, GetNameSafe(NewWidget));
		}

	}
}

UZSCustomButton* UResolutionMenuAnchorClass::CreateButton(const FButtonStyle ButtonStyle, EResolution Resolution)
{
	UZSCustomButton* Button = NewObject<UZSCustomButton>(this);
	if (IsValid(Button))
	{
		Button->SetStyle(ButtonStyle);
		ResolutionAndButton.Add(Resolution, Button);	
	}

	return Button;
}

void UResolutionMenuAnchorClass::SetResolutionAndWidgetDataAsset(UResolutionAndWidgetDataAsset* NewValue)
{
	ResolutionAndWidgetDataAsset = NewValue;
}

void UResolutionMenuAnchorClass::CreateDefaultButtons()
{
	if (IsValid(ButtonStyleByResolution) && IsValid(VerticalBox_Resolutions))
	{
		const EResolution CurrentResolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(this);
		const TArray<EResolution> Resolutions = UUIBlueprintFunctionLibrary::GetAllResolutionExceptChoseResolution(this, CurrentResolution);

		if (VerticalBox_Resolutions->GetAllChildren().Num() < Resolutions.Num())
		{
			for (EResolution Res : Resolutions)
			{
				UZSCustomButton* NewButton = CreateButton(FButtonStyle(), Res);
				if (IsValid(NewButton))
				{
					NewButton->Resolution = Res;
					NewButton->OnClickedReturnThis.AddUniqueDynamic(this, &UResolutionMenuAnchorClass::OnSelectResolution);
					
					UVerticalBoxSlot* VB_Slot = Cast<UVerticalBoxSlot>(VerticalBox_Resolutions->AddChild(NewButton));
					if (VB_Slot)
					{
						VB_Slot->SetSize(FSlateChildSize());
						auto ButtonStyle = UButtonStyleByResolution::GetButtonStyleByResolution(this, Res, EResolutionButtonStyleType::NormalStyle, ButtonStyleByResolution);
						NewButton->SetStyle(ButtonStyle);
					}
				}
			}
		}
	}
}
