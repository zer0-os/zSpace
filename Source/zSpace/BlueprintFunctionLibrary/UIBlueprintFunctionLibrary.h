// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "zSpace/Types/UITypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (WorldContext="WorldContext"))
	static TSubclassOf<class UUserWidget> GetWidgetSubClassForCurrentScreen(const UObject* WorldContext, class UResolutionAndWidgetDataAsset* WidgetDataAsset);

	UFUNCTION(BlueprintPure, meta = (WorldContext="WorldContext"))
	static FIntPoint GetCurrentScreenResolution(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, meta = (WorldContext="WorldContext"))
	static EResolution GetCurrentScreenResolutionEnum(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, meta = (WorldContext="WorldContext"))
	static class UUserWidget* GetWidgetByWidgetType(const UObject* WorldContext, EWidgetType WidgetType);
};
