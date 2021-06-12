// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class AVirtualKeyboardActor * VirtualKeyboardActor = nullptr;
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWidgetInteractionComponent(class UWidgetInteractionComponent * NewWidgetInteractionComponent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetInteractionComponent, class UWidgetInteractionComponent *, NewWidgetInteractionComponent);

	UPROPERTY(BlueprintAssignable)
	FSetWidgetInteractionComponent OnSetWidgetInteractionComponent;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	class UWidgetInteractionComponent * GetWidgetInteractionComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetVirtualKeyboardActor(class AVirtualKeyboardActor * NewVirtualKeyboardActor);
	
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetVirtualKeyboardActor, class AVirtualKeyboardActor *, NewVirtualKeyboardActor);

	//UPROPERTY(BlueprintAssignable)
	//FSetVirtualKeyboardActor OnSetVirtualKeyboardActor;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EventSetVirtualKeyboardActor();

private:

	UFUNCTION(BlueprintCallable)
	void SetVirtualKeyboardActorInKey(TArray<UHorizontalBox*> NewHorizontalBox);

	UFUNCTION(BlueprintCallable)
	void SetWidgetInteractionComponentInKey(TArray<UHorizontalBox*> NewHorizontalBox);
	
	
};
