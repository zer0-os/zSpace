// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
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
	static TSubclassOf<class UUserWidget> GetPreLoginWidgetForCurrentScreen(const UObject* WorldContext, class UResolutionAndWidget* PreLoginDataAsset);
};
