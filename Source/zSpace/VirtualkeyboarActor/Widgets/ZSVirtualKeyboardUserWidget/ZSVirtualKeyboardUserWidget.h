// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/VirtualkeyboarActor/VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"

#include "ZSVirtualKeyboardUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ZSPACE_API UZSVirtualKeyboardUserWidget : public UUserWidget , public  IVirtualKeyboardWidgetInterface
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponent = nullptr;
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWidgetInteractionComponent(class UWidgetInteractionComponent * NewWidgetInteractionComponent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetInteractionComponent, class UWidgetInteractionComponent *, NewWidgetInteractionComponent);

	UPROPERTY(BlueprintAssignable)
	FSetWidgetInteractionComponent OnSetWidgetInteractionComponent;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UWidgetInteractionComponent * GetWidgetInteractionComponent();
	
};
