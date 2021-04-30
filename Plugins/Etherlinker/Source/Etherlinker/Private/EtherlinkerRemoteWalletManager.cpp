// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "EtherlinkerRemoteWalletManager.h"
#include "EtherlinkerSettings.h"

UEtherlinkerRemoteWalletManager::UEtherlinkerRemoteWalletManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UEtherlinkerRemoteWalletManager::GetWalletData(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerGetWalletData(walletAuthenticationRequest);
		return;
	}

	MakeRequest(walletAuthenticationRequest, "/getWalletData");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("GetWalletData executed"));
#endif

}

void UEtherlinkerRemoteWalletManager::ServerGetWalletData_Implementation(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	GetWalletData(walletAuthenticationRequest);
}

bool UEtherlinkerRemoteWalletManager::ServerGetWalletData_Validate(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	return true;
}

void UEtherlinkerRemoteWalletManager::CreateUserAccount(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerCreateUserAccount(walletAuthenticationRequest);
		return;
	}

	if (walletAuthenticationRequest.walletPath.IsEmpty()) {

		UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
		check(EtherlinkerSettings);

		if (!EtherlinkerSettings->DefaultWalletPath.IsEmpty()) {
			walletAuthenticationRequest.walletPath = EtherlinkerSettings->DefaultWalletPath;
		}
		else {
			walletAuthenticationRequest.walletPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()).Append("EtherlinkerKeys/");
		}
	}

	MakeRequest(walletAuthenticationRequest, "/createUserAccount");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("CreateUserAccount executed"));
#endif

}

void UEtherlinkerRemoteWalletManager::ServerCreateUserAccount_Implementation(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	CreateUserAccount(walletAuthenticationRequest);
}

bool UEtherlinkerRemoteWalletManager::ServerCreateUserAccount_Validate(FWalletAuthenticationRequest walletAuthenticationRequest)
{
	return true;
}

bool UEtherlinkerRemoteWalletManager::MakeRequest(FWalletAuthenticationRequest WalletAuthenticationRequest, const FString& URL)
{
	// Get actual server address from settings
	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);

	FString serverAddress;
	if (WalletAuthenticationRequest.serverAddress.IsEmpty()) {
		serverAddress = EtherlinkerSettings->IntegrationServerURL;
	}
	else {
		serverAddress = WalletAuthenticationRequest.serverAddress;
	}

	TSharedPtr<FJsonObject> JsontRequestObject = FJsonObjectConverter::UStructToJsonObject(WalletAuthenticationRequest);

	FString OutputString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsontRequestObject.ToSharedRef(), JsonWriter);
	
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OutputString);
#endif

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");

	HttpRequest->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetHeader("SenderId", WalletAuthenticationRequest.senderId);
	HttpRequest->SetHeader("UserIndex", WalletAuthenticationRequest.userIndex);
	HttpRequest->SetURL(serverAddress + URL);
	HttpRequest->SetContentAsString(OutputString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEtherlinkerRemoteWalletManager::OnResponseReceived);
	HttpRequest->ProcessRequest();

	return true;
}

void UEtherlinkerRemoteWalletManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	
	if (!bWasSuccessful || !Response.IsValid())
	{

		// If we can't connect to the server or in case of any other error
		FWalletAuthenticationResponse walletAuthenticationResponse;
		walletAuthenticationResponse.data = "Unknown network error in processing HTTP response.";

		if (!Request->GetHeader("UserIndex").IsEmpty())
		{
			walletAuthenticationResponse.userIndex = Request->GetHeader("UserIndex");
			UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
		}

		if (!Request->GetHeader("SenderId").IsEmpty())
		{
			walletAuthenticationResponse.senderId = Request->GetHeader("SenderId");
			UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
		}
		else {
			walletAuthenticationResponse.senderId = "DefaultErrorProcessor";
			UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
		}

		OnResponseReceivedEvent.Broadcast("error", walletAuthenticationResponse);
		UE_LOG(LogTemp, Error, TEXT("Unknown network error in processing HTTP response."));

	}
	else
	{
		//Create a pointer to hold the json serialized data
		TSharedPtr<FJsonObject> JsonObject;

		//Create a reader pointer to read the json data
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		//Deserialize the json data given Reader and the actual object to deserialize
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

			FString result = JsonObject->GetStringField("result");

#if !UE_BUILD_SHIPPING
			UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
#endif

			// Process error response
			if (result.Equals("error")) {
				FString errorText = JsonObject->GetStringField("data");
				FWalletAuthenticationResponse walletAuthenticationResponse;
				walletAuthenticationResponse.data = errorText;

				if (!Request->GetHeader("UserIndex").IsEmpty())
				{
					walletAuthenticationResponse.userIndex = Request->GetHeader("UserIndex");
					UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
				}

				if (!Request->GetHeader("SenderId").IsEmpty())
				{
					walletAuthenticationResponse.senderId = Request->GetHeader("SenderId");
					UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
				}
				else {
					walletAuthenticationResponse.senderId = "DefaultErrorProcessor";
					UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
				}
				UE_LOG(LogTemp, Error, TEXT("%s"), *walletAuthenticationResponse.data);

				OnResponseReceivedEvent.Broadcast(result, walletAuthenticationResponse);
			}
			else {
				// Process normal response
				TSharedPtr<FJsonObject> JsonResponseObject = JsonObject->GetObjectField("data");
				FWalletAuthenticationResponse walletAuthenticationResponse;
				FJsonObjectConverter::JsonAttributesToUStruct(JsonResponseObject->Values, FWalletAuthenticationResponse::StaticStruct(), &walletAuthenticationResponse, 0, 0);

#if !UE_BUILD_SHIPPING
				UE_LOG(LogTemp, Warning, TEXT("%s"), *walletAuthenticationResponse.data);
#endif

				OnResponseReceivedEvent.Broadcast(result, walletAuthenticationResponse);
			}

		}
		else {
			// If we can't deserialize JSON response
			FWalletAuthenticationResponse walletAuthenticationResponse;
			walletAuthenticationResponse.data = "Can't deserialize JSON response.";

			if (!Request->GetHeader("UserIndex").IsEmpty())
			{
				walletAuthenticationResponse.userIndex = Request->GetHeader("UserIndex");
				UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
			}

			if (!Request->GetHeader("SenderId").IsEmpty())
			{
				walletAuthenticationResponse.senderId = Request->GetHeader("SenderId");
				UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
			}
			else {
				walletAuthenticationResponse.senderId = "DefaultErrorProcessor";
				UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
			}

			OnResponseReceivedEvent.Broadcast("error", walletAuthenticationResponse);
			UE_LOG(LogTemp, Error, TEXT("Can't deserialize JSON response."));
		}		
	}
	
}
