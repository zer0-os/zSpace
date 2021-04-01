// Copyright 2020 Sabre Dart Studios


#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include <GameFramework/GameUserSettings.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/UserWidget.h>
#include "../Types/UITypes.h"

TSubclassOf<class UUserWidget>UUIBlueprintFunctionLibrary::GetWidgetSubClassForCurrentScreen(const UObject* WorldContext, UResolutionAndWidgetDataAsset* PreLoginDataAsset)
{
	check(PreLoginDataAsset);
	if (!IsValid(PreLoginDataAsset)) return UUserWidget::StaticClass();

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	check(UserSettings);
	if (!IsValid(UserSettings)) return UUserWidget::StaticClass();

	// Get Current Resolution
	FIntPoint ScreenResolution = UserSettings->GetScreenResolution();

	// Get Enum Resolution For Current Resolution
	EResolution Resolution = UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
	if (Resolution == EResolution::None)
	{
		Resolution = EResolution::R_1920X1080;
	}

	// Get Widget For Current Resolution
	TSubclassOf<UUserWidget> Result = PreLoginDataAsset->GetWidget(Resolution);

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
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	check(UserSettings);
	if (!IsValid(UserSettings)) return EResolution::R_1920X1080;

	FIntPoint ScreenResolution = UserSettings->GetScreenResolution();
	EResolution Resolution = UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(WorldContext, ScreenResolution);
	if (Resolution == EResolution::None)
	{
		Resolution = EResolution::R_1920X1080;
	}
	return Resolution;
}
