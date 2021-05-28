// Copyright 2020 Sabre Dart Studios


#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"

#include "../Components/ManageWidgetsResolution.h"
#include <GameFramework/GameUserSettings.h>
#include <Kismet/KismetSystemLibrary.h>
#include "../Game/ZSpaceGameInstance.h"
#include <Blueprint/UserWidget.h>
#include "../Types/UITypes.h"
#include <Engine/World.h>
#include "zSpace/zSpace.h"

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
	UWorld* World = WorldContext->GetWorld();
	if (IsValid(World))
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (IsValid(PC))
		{
			int32 SizeX, SizeY;
			PC->GetViewportSize(SizeX, SizeY);
			return FIntPoint(SizeX, SizeY);
		}
	}
	
	return FIntPoint::ZeroValue;
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
	case EResolution::R_5120X1440: ResolutionString = "5120x1440"; break;
	case EResolution::R_1920X1080: ResolutionString = "1920x1080"; break;
	case EResolution::R_1440X900: ResolutionString = "1440x900"; break;
	default: ResolutionString = "1920x1080"; break;
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
