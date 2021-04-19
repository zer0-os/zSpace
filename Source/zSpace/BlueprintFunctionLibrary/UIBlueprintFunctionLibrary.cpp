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
	EResolution Resolution = UConvertEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
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
	EResolution Resolution = UConvertEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
	if (Resolution == EResolution::None)
	{
		Resolution = EResolution::R_1920X1080;
	}
	return Resolution;
}

TArray<EResolution> UUIBlueprintFunctionLibrary::GetAllResolutionExceptChoseResolution(const UObject* WorldContext,
	EResolution SelectResolution)
{
	TArray<EResolution> Result;

	const uint8 Start = 1;
	const uint8 End = static_cast<uint8>(EResolution::MAX);

	for (uint8 X(Start); X < End; X++)
	{
		if (SelectResolution != static_cast<EResolution>(X))
		{
			Result.Add(static_cast<EResolution>(X));
		}
	}
	
	return Result;
}

FString UUIBlueprintFunctionLibrary::GetResolutionByEnum(const UObject* WorldContext, const EResolution Resolution, TEnumAsByte<EWindowMode::Type> WindowMode)
{
	auto GetWindowMode = [WindowMode]() -> char
	{
		switch (WindowMode)
		{
		case EWindowMode::Type::Fullscreen: return 'f';
		case EWindowMode::Type::Windowed: return 'w';
		case EWindowMode::Type::WindowedFullscreen: return 'w';
		default: return '\0';
		}	
	};

	FString ResolutionString;
	
	switch (Resolution)
	{
	case EResolution::R_5120X1440: return ResolutionString = "5120x1440";
	case EResolution::R_1920X1080: return ResolutionString = "1920x1080";
	case EResolution::R_1440X900: return ResolutionString = "1440x900";
	default: ResolutionString = "1920x1080";
	}

	return ResolutionString + GetWindowMode();
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
			EResolution Resolution = UConvertEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
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
