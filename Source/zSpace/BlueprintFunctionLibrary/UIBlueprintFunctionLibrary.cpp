// Copyright 2020 Sabre Dart Studios


#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include <GameFramework/GameUserSettings.h>
#include <Kismet/KismetSystemLibrary.h>
#include "../Game/ZSpaceGameInstance.h"
#include <Blueprint/UserWidget.h>
#include "../Types/UITypes.h"
#include <Engine/World.h>
#include "../Components/ManageWidgetsResolution.h"

TSubclassOf<class UUserWidget>UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(const UObject* WorldContext, UResolutionAndWidgetDataAsset* WidgetDataAsset)
{
	check(WidgetDataAsset);
	if (!IsValid(WidgetDataAsset)) return UUserWidget::StaticClass();

	// Get Current Resolution
	const FIntPoint ScreenResolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolution(WorldContext);

	// Get Enum Resolution For Current Resolution
	EResolution Resolution = UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
	if (Resolution == EResolution::None)
	{
		Resolution = EResolution::R_1920X1080;
	}

	// Get Widget For Current Resolution
	TSubclassOf<UUserWidget> Result = WidgetDataAsset->GetWidget(Resolution);

	return Result ? Result : UUserWidget::StaticClass();
}

FIntPoint UUIBlueprintFunctionLibrary::GetCurrentScreenResolution(const UObject* WorldContext)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	check(UserSettings);
	if (!IsValid(UserSettings)) return FIntPoint::ZeroValue;

	return UserSettings->GetScreenResolution();
}

EResolution UUIBlueprintFunctionLibrary::GetCurrentScreenResolutionEnum(const UObject* WorldContext)
{
	const FIntPoint ScreenResolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolution(WorldContext);
	EResolution Resolution = UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
	if (Resolution == EResolution::None)
	{
		Resolution = EResolution::R_1920X1080;
	}
	return Resolution;
}

class UUserWidget* UUIBlueprintFunctionLibrary::GetWidgetByWidgetType(const UObject* WorldContext, EWidgetType WidgetType)
{
	if (!IsValid(GEngine)) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContext);
	if (!IsValid(World)) return nullptr;

	UZSpaceGameInstance* GameInstance = World->GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
		UManageWidgetsResolution* ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
		if (IsValid(ManageWidgetsResolution))
		{
			const FIntPoint ScreenResolution = UUIBlueprintFunctionLibrary::GetCurrentScreenResolution(WorldContext);
			EResolution Resolution = UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
			if (Resolution == EResolution::None)
			{
				Resolution = EResolution::R_1920X1080;
			}
			
			UUserWidget* Widget = ManageWidgetsResolution->GetWidgetByResolution(WidgetType, Resolution);
			return Widget;
		}
	}

	return nullptr;
}
