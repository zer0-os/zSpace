// Copyright 2020 Sabre Dart Studios


#include "zSpace/BlueprintFunctionLibrary/UIBlueprintFunctionLibrary.h"
#include <GameFramework/GameUserSettings.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/UserWidget.h>
#include "../Types/UITypes.h"

TSubclassOf<class UUserWidget>UUIBlueprintFunctionLibrary::GetPreLoginWidgetForCurrentScreen(const UObject* WorldContext, UResolutionAndWidget* PreLoginDataAsset)
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
