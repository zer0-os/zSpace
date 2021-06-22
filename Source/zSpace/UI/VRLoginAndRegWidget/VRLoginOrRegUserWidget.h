// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRLoginOrRegUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UVRLoginOrRegUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UButton * ButtonLogin = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UButton * ButtonReg = nullptr;

	class UVRLoginAndRegWidget * VRLoginAndRegWidget = nullptr;
	
	friend class UVRLoginAndRegWidget;


public:


	UFUNCTION()
	void OnClickButtonLogin();

	UFUNCTION()
	void OnClickButtonReg();
	
	
};
