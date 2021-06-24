// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/VirtualkeyboarActor/VirtualKeyboardActor.h"
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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class AVirtualKeyboardActor * VirtualKeyboardActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FVirtualKeyboardKey Key;


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	class UButton * KeyButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	class UTextBlock * TextBlockKeyName = nullptr;

protected:

	virtual void NativePreConstruct() override;

	void SetKeyText();
	
public:

	UFUNCTION()
	void PressedShift(const bool NewIsPressedShift);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWidgetInteractionComponent(class UWidgetInteractionComponent * NewWidgetInteractionComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetVirtualKeyboardActor(class AVirtualKeyboardActor * NewVirtualKeyboardActor);

	UFUNCTION()
	void PressKey();

};
