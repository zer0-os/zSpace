// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Types/UITypes.h"
#include "UIResolutionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIResolutionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IUIResolutionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EWidgetType GetWidgetType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UResolutionAndWidgetDataAsset* GetToPreviousMenuDataAsset();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FBackgroundVideo GetBackgroundVideo();
};
