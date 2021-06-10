// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/VirtualkeyboarActor/VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"
#include "ZSKeyboardKeyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSKeyboardKeyUserWidget : public UUserWidget, public IVirtualKeyboardWidgetInterface
{
	GENERATED_BODY()

private:	

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponent = nullptr;
	
public:

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWidgetInteractionComponent(class UWidgetInteractionComponent * NewWidgetInteractionComponent);


};
