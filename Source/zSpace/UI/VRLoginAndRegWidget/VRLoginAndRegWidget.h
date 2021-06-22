// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"

#include "VRLoginAndRegWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UVRLoginAndRegWidget : public UOWSLoginWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UWidgetSwitcher * WidgetSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UVRLoginUserWidget * VRLoginUserWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UVRRegUserWidget * VRRegUserWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidgetOptional))
	class UVRLoginOrRegUserWidget * VRLoginOrRegUserWidget  = nullptr;

	friend class UVRLoginOrRegUserWidget;
	friend class UVRLoginUserWidget;
	friend class UVRRegUserWidget;


public:

	UVRLoginAndRegWidget(const class FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void NativeConstruct() override;
	
	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FString TempLogin;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FString TempPassword;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 bIsReg:1;
	
public:

	void Login(FString NewLogin, FString newPassword);

	void Reg(FString NewEmail, FString NewPassword, FString NewFirstName, FString NewLastName);

	UFUNCTION(BlueprintCallable)
	void LoginSuccess(FString  NewUserSessionGUID);

	UFUNCTION(BlueprintCallable)
	void RegSuccess();

	void SwitchLoginWidget();

	void SwitchRegWidget();

	void SwitchLoginOrRegUserWidget();


		
};
