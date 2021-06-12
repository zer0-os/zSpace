// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VirtualKeyboardWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVirtualKeyboardWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZSPACE_API IVirtualKeyboardWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWidgetInteractionComponent(class UWidgetInteractionComponent * NewWidgetInteractionComponent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UWidgetInteractionComponent * GetWidgetInteractionComponent();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetVirtualKeyboardActor(class AVirtualKeyboardActor * NewVirtualKeyboardActor);
	
};
