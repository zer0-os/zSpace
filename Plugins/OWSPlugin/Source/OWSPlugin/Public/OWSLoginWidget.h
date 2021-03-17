// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class OWSPLUGIN_API UOWSLoginWidget : public UUserWidget
{
	GENERATED_BODY()

	FHttpModule* Http;
	
public:	

	UOWSLoginWidget(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
		FString RPGAPICustomerKey;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPIPath = "";

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString OWS2APIPath = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login")
		float LoginTimeout = 20.f;
	/*
	UFUNCTION(BlueprintCallable, Category = "Login")
	void Login(FString Email, FString Password);

	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorLogin(const FString &ErrorMsg);
	*/

	//LoginAndCreateSession
	UFUNCTION(BlueprintCallable, Category = "Login")
		void LoginAndCreateSession(FString Email, FString Password);

	void OnLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyLoginAndCreateSession(const FString &UserSessionGUID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorLoginAndCreateSession(const FString &ErrorMsg);

	//ExternalLoginAndCreateSession
	UFUNCTION(BlueprintCallable, Category = "Login")
		void ExternalLoginAndCreateSession(FString Email, FString Password);

	void OnExternalLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyExternalLoginAndCreateSession(const FString &UserSessionGUID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorExternalLoginAndCreateSession(const FString &ErrorMsg);

	//Register
	UFUNCTION(BlueprintCallable, Category = "Login")
		void Register(FString Email, FString Password, FString FirstName, FString LastName);

	void OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyRegister();

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorRegister(const FString &ErrorMsg);

	//OpenCreateAccountInBrowser
	UFUNCTION(BlueprintCallable, Category = "Login")
		void OpenCreateAccountInBrowser();
	

};
