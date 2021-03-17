// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSLoginWidget.h"
#include "OWSPlugin.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSPlayerController.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"

UOWSLoginWidget::UOWSLoginWidget(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("OWS2APIPath"),
		OWS2APIPath,
		GGameIni
	);
}

void UOWSLoginWidget::LoginAndCreateSession(FString Email, FString Password)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(LoginTimeout); //Set timeout

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSLoginWidget::OnLoginAndCreateSessionResponseReceived);

	FString url = FString(OWS2APIPath + "api/Users/LoginAndCreateSession");

	//Trim whitespace
	Email.TrimStartAndEndInline();
	Password.TrimStartAndEndInline();

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(Email);
	FormatParams.Add(Password);
	FString PostParameters = FString::Format(TEXT("{ \"Email\": \"{0}\", \"Password\": \"{1}\" }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);	
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSLoginWidget::OnLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ErrorMessage = JsonObject->GetStringField("ErrorMessage");

			if (!ErrorMessage.IsEmpty())
			{
				ErrorLoginAndCreateSession(*ErrorMessage);
				return;
			}

			FString UserSessionGUID = JsonObject->GetStringField("UserSessionGUID");

			NotifyLoginAndCreateSession(UserSessionGUID);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnUserLoginResponseReceived Server returned no data!"));
			ErrorLoginAndCreateSession(TEXT("Unknown login error!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnUserLoginResponseReceived Error accessing login server!"));
		ErrorLoginAndCreateSession(TEXT("Unknown error connecting to server!"));
	}
}


void UOWSLoginWidget::ExternalLoginAndCreateSession(FString Email, FString Password)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(LoginTimeout); //Set timeout

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSLoginWidget::OnExternalLoginAndCreateSessionResponseReceived);

	FString url = FString(OWS2APIPath + "api/Users/ExternalLoginAndCreateSession");

	//Trim whitespace
	Email.TrimStartAndEndInline();
	Password.TrimStartAndEndInline();

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(Email);
	FormatParams.Add(Password);
	FString PostParameters = FString::Format(TEXT("{ \"Email\": \"{0}\", \"Password\": \"{1}\" }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSLoginWidget::OnExternalLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ErrorMessage = JsonObject->GetStringField("ErrorMessage");

			if (!ErrorMessage.IsEmpty())
			{
				ErrorExternalLoginAndCreateSession(*ErrorMessage);
				return;
			}

			FString UserSessionGUID = JsonObject->GetStringField("UserSessionGUID");

			NotifyExternalLoginAndCreateSession(UserSessionGUID);
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnUserLoginResponseReceived Server returned no data!"));
			ErrorExternalLoginAndCreateSession(TEXT("Unknown login error!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnUserLoginResponseReceived Error accessing login server!"));
		ErrorExternalLoginAndCreateSession(TEXT("Unknown error connecting to server!"));
	}
}



void UOWSLoginWidget::Register(FString Email, FString Password, FString FirstName, FString LastName)
{
	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(LoginTimeout); //Set timeout

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSLoginWidget::OnRegisterResponseReceived);

	FString url = FString(OWS2APIPath + "api/Users/RegisterUser");

	//Trim whitespace
	Email.TrimStartAndEndInline();
	Password.TrimStartAndEndInline();

	TArray<FStringFormatArg> FormatParams;
	FormatParams.Add(Email);
	FormatParams.Add(Password);
	FormatParams.Add(FirstName);
	FormatParams.Add(LastName);
	FString PostParameters = FString::Format(TEXT("{ \"Email\": \"{0}\", \"Password\": \"{1}\", \"FirstName\": \"{2}\", \"LastName\": \"{3}\" }"), FormatParams);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader(TEXT("X-CustomerGUID"), RPGAPICustomerKey);
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSLoginWidget::OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ErrorMessage = JsonObject->GetStringField("ErrorMessage");

			if (!ErrorMessage.IsEmpty())
			{
				ErrorRegister(*ErrorMessage);
				return;
			}

			NotifyRegister();
		}
		else
		{
			UE_LOG(OWS, Error, TEXT("OnRegisterResponseReceived Server returned no data!"));
			ErrorRegister(TEXT("Unknown registration error!"));
		}
	}
	else
	{
		UE_LOG(OWS, Error, TEXT("OnRegisterResponseReceived Error accessing login server!"));
		ErrorRegister(TEXT("Unknown error connecting to server!"));
	}
}




void UOWSLoginWidget::OpenCreateAccountInBrowser()
{
	FString TheURL = "http://" + RPGAPIPath + "/Login/CreateAccount/" + RPGAPICustomerKey;
	FPlatformProcess::LaunchURL(*TheURL, nullptr, nullptr);
}