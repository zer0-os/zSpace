// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "EtherManager.h"
#include "EtherlinkerSettings.h"

UEtherManager::UEtherManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEtherManager::GetWalletBalance(FEtherlinkerRequestData etherlinkerRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerGetWalletBalance(etherlinkerRequestData);
		return;
	}

	MakeRequest(etherlinkerRequestData, "/getWalletBalance");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("GetWalletBalance executed"));
#endif

}

void UEtherManager::ServerGetWalletBalance_Implementation(FEtherlinkerRequestData etherlinkerRequestData)
{
	GetWalletBalance(etherlinkerRequestData);
}

bool UEtherManager::ServerGetWalletBalance_Validate(FEtherlinkerRequestData etherlinkerRequestData)
{
	return true;
}

void UEtherManager::TransferEther(FEtherlinkerRequestData etherlinkerRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerTransferEther(etherlinkerRequestData);
		return;
	}

	etherlinkerRequestData = AddWalletDataToRequest(etherlinkerRequestData);
	MakeRequest(etherlinkerRequestData, "/transferEther");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("TransferEther executed"));
#endif

}


void UEtherManager::ServerTransferEther_Implementation(FEtherlinkerRequestData etherlinkerRequestData)
{
	TransferEther(etherlinkerRequestData);
}

bool UEtherManager::ServerTransferEther_Validate(FEtherlinkerRequestData etherlinkerRequestData)
{
	return true;
}

void UEtherManager::ExecContractMethod(FEtherlinkerRequestData etherlinkerRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerExecContractMethod(etherlinkerRequestData);
		return;
	}

	etherlinkerRequestData = AddWalletDataToRequest(etherlinkerRequestData);
	MakeRequest(etherlinkerRequestData, "/execContractMethod");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("ExecContractMethod executed"));
#endif

}

void UEtherManager::ServerExecContractMethod_Implementation(FEtherlinkerRequestData etherlinkerRequestData)
{
	ExecContractMethod(etherlinkerRequestData);
}

bool UEtherManager::ServerExecContractMethod_Validate(FEtherlinkerRequestData etherlinkerRequestData)
{
	return true;
}

void UEtherManager::DeployContract(FEtherlinkerRequestData etherlinkerRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerDeployContract(etherlinkerRequestData);
		return;
	}
	etherlinkerRequestData = AddWalletDataToRequest(etherlinkerRequestData);
	MakeRequest(etherlinkerRequestData, "/deployContract");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("DeployContract executed"));
#endif

}

void UEtherManager::ServerDeployContract_Implementation(FEtherlinkerRequestData etherlinkerRequestData)
{
	DeployContract(etherlinkerRequestData);
}

bool UEtherManager::ServerDeployContract_Validate(FEtherlinkerRequestData etherlinkerRequestData)
{
	return true;
}

void UEtherManager::CreateWallet(FEtherlinkerRequestData etherlinkerRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerCreateWallet(etherlinkerRequestData);
		return;
	}

	etherlinkerRequestData = AddWalletDataToRequest(etherlinkerRequestData);
	MakeRequest(etherlinkerRequestData, "/createWallet");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("CreateWallet executed"));
#endif

}

void UEtherManager::ServerCreateWallet_Implementation(FEtherlinkerRequestData etherlinkerRequestData)
{
	CreateWallet(etherlinkerRequestData);
}

bool UEtherManager::ServerCreateWallet_Validate(FEtherlinkerRequestData etherlinkerRequestData)
{
	return true;
}

void UEtherManager::SetWalletData(FWalletData newWalletData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerSetWalletData(newWalletData);
		return;
	}

	walletData = newWalletData;
}

void UEtherManager::ServerSetWalletData_Implementation(FWalletData newWalletData)
{
	SetWalletData(newWalletData);
}

bool UEtherManager::ServerSetWalletData_Validate(FWalletData newWalletData)
{
	return true;
}

void UEtherManager::ProcessBatchRequest(FEtherlinkerBatchRequestData etherlinkerBatchRequestData)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) {
		ServerProcessBatchRequest(etherlinkerBatchRequestData);
		return;
	}

	for (int i = 0; i < etherlinkerBatchRequestData.etherlinkerRequestDataList.Num(); i++) {
		etherlinkerBatchRequestData.etherlinkerRequestDataList[i] = AddWalletDataToRequest(etherlinkerBatchRequestData.etherlinkerRequestDataList[i]);
	}
	
	MakeBatchRequest(etherlinkerBatchRequestData, "/processBatchRequest");

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("ProcessBatchRequest executed"));
#endif

}

void UEtherManager::ServerProcessBatchRequest_Implementation(FEtherlinkerBatchRequestData etherlinkerBatchRequestData)
{
	ProcessBatchRequest(etherlinkerBatchRequestData);
}

bool UEtherManager::ServerProcessBatchRequest_Validate(FEtherlinkerBatchRequestData etherlinkerBatchRequestData)
{
	return true;
}

FEtherlinkerRequestData UEtherManager::AddWalletDataToRequest(FEtherlinkerRequestData etherlinkerRequestData) const
{
	etherlinkerRequestData.walletAddress = walletData.walletAddress;
	etherlinkerRequestData.walletMnemonic = walletData.walletMnemonic;
	etherlinkerRequestData.walletPassword = walletData.walletPassword;
	etherlinkerRequestData.walletAuthType = walletData.walletAuthType;

	if (walletData.walletPath.IsEmpty()) {

		UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
		check(EtherlinkerSettings);

		if (!EtherlinkerSettings->DefaultWalletPath.IsEmpty()) {
			etherlinkerRequestData.walletPath = EtherlinkerSettings->DefaultWalletPath;
		}
		else {
			etherlinkerRequestData.walletPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()).Append("EtherlinkerKeys/");
		}
		
	}
	else {
		etherlinkerRequestData.walletPath = walletData.walletPath;
	}

	return etherlinkerRequestData;
}

bool UEtherManager::MakeRequest(FEtherlinkerRequestData EtherlinkerRequestData, const FString& URL)
{
	// Get actual server address from settings
	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);

	FString serverAddress;
	if (EtherlinkerRequestData.serverAddress.IsEmpty()) {
		serverAddress = EtherlinkerSettings->IntegrationServerURL;
	}
	else {
		serverAddress = EtherlinkerRequestData.serverAddress;
	}

	// Check infura URL
	if (EtherlinkerRequestData.InfuraURL.IsEmpty()) {
		if (EtherlinkerSettings->InfuraURL.IsEmpty()) {
			UE_LOG(LogTemp, Error, TEXT("Infura URL is empty. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server."));
		}
		else {
			EtherlinkerRequestData.InfuraURL = EtherlinkerSettings->InfuraURL;
		}
	}

	TSharedPtr<FJsonObject> JsontRequestObject = FJsonObjectConverter::UStructToJsonObject(EtherlinkerRequestData);

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
	HttpRequest->SetHeader("SenderId", EtherlinkerRequestData.senderId);
	HttpRequest->SetHeader("UserIndex", EtherlinkerRequestData.userIndex);
	HttpRequest->SetURL(serverAddress + URL);
	HttpRequest->SetContentAsString(OutputString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEtherManager::OnResponseReceived);
	HttpRequest->ProcessRequest();

	return true;
}

bool UEtherManager::MakeBatchRequest(FEtherlinkerBatchRequestData EtherlinkerBatchRequestData, const FString& URL)
{
	// Get actual server address from settings
	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);

	FString serverAddress;
	if (EtherlinkerBatchRequestData.serverAddress.IsEmpty()) {
		serverAddress = EtherlinkerSettings->IntegrationServerURL;
	}
	else {
		serverAddress = EtherlinkerBatchRequestData.serverAddress;
	}

	// Check infura URL
	if (EtherlinkerBatchRequestData.InfuraURL.IsEmpty()) {
		if (EtherlinkerSettings->InfuraURL.IsEmpty()) {
			UE_LOG(LogTemp, Error, TEXT("Infura URL is empty. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server."));
		}
		else {
			EtherlinkerBatchRequestData.InfuraURL = EtherlinkerSettings->InfuraURL;
			for (int i = 0; i < EtherlinkerBatchRequestData.etherlinkerRequestDataList.Num(); i++) {
				EtherlinkerBatchRequestData.etherlinkerRequestDataList[i].InfuraURL = EtherlinkerSettings->InfuraURL;
			}
		}
	}

	TSharedPtr<FJsonObject> JsontRequestObject = FJsonObjectConverter::UStructToJsonObject(EtherlinkerBatchRequestData);

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
	HttpRequest->SetHeader("SenderId", EtherlinkerBatchRequestData.senderId);
	HttpRequest->SetHeader("UserIndex", EtherlinkerBatchRequestData.userIndex);
	HttpRequest->SetURL(serverAddress + URL);
	HttpRequest->SetContentAsString(OutputString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEtherManager::OnBatchResponseReceived);
	HttpRequest->ProcessRequest();

	return true;
}

void UEtherManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	
	if (!bWasSuccessful || !Response.IsValid())
	{

		// If we can't connect to the server or in case of any other error
		FEtherlinkerResponseData etherlinkerResponseData;
		etherlinkerResponseData.data = "Unknown network error in processing HTTP response.";

		if (!Request->GetHeader("UserIndex").IsEmpty())
		{
			etherlinkerResponseData.userIndex = Request->GetHeader("UserIndex");
			UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
		}

		if (!Request->GetHeader("SenderId").IsEmpty())
		{
			etherlinkerResponseData.senderId = Request->GetHeader("SenderId");
			UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
		}
		else {
			etherlinkerResponseData.senderId = "DefaultErrorProcessor";
			UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
		}

		OnResponseReceivedEvent.Broadcast("error", etherlinkerResponseData);
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
				FEtherlinkerResponseData etherlinkerResponseData;
				etherlinkerResponseData.data = errorText;

				if (!Request->GetHeader("UserIndex").IsEmpty())
				{
					etherlinkerResponseData.userIndex = Request->GetHeader("UserIndex");
					UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
				}

				if (!Request->GetHeader("SenderId").IsEmpty())
				{
					etherlinkerResponseData.senderId = Request->GetHeader("SenderId");
					UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
				}
				else {
					etherlinkerResponseData.senderId = "DefaultErrorProcessor";
					UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
				}

				UE_LOG(LogTemp, Error, TEXT("%s"), *etherlinkerResponseData.data);

				OnResponseReceivedEvent.Broadcast(result, etherlinkerResponseData);
			}
			else {
				// Process normal response
				TSharedPtr<FJsonObject> JsonResponseObject = JsonObject->GetObjectField("data");
				FEtherlinkerResponseData etherlinkerResponseData;
				FJsonObjectConverter::JsonAttributesToUStruct(JsonResponseObject->Values, FEtherlinkerResponseData::StaticStruct(), &etherlinkerResponseData, 0, 0);

#if !UE_BUILD_SHIPPING
				UE_LOG(LogTemp, Warning, TEXT("%s"), *etherlinkerResponseData.data);
#endif

				OnResponseReceivedEvent.Broadcast(result, etherlinkerResponseData);
			}

		}
		else {
			// If we can't deserialize JSON response
			FEtherlinkerResponseData etherlinkerResponseData;
			etherlinkerResponseData.data = "Can't deserialize JSON response.";

			if (!Request->GetHeader("UserIndex").IsEmpty())
			{
				etherlinkerResponseData.userIndex = Request->GetHeader("UserIndex");
				UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
			}

			if (!Request->GetHeader("SenderId").IsEmpty())
			{
				etherlinkerResponseData.senderId = Request->GetHeader("SenderId");
				UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
			}
			else {
				etherlinkerResponseData.senderId = "DefaultErrorProcessor";
				UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
			}

			OnResponseReceivedEvent.Broadcast("error", etherlinkerResponseData);
			UE_LOG(LogTemp, Error, TEXT("Can't deserialize JSON response."));
		}		
	}
	
}

void UEtherManager::OnBatchResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{

		// If we can't connect to the server or in case of any other error
		FEtherlinkerBatchResponseData etherlinkerBatchResponseData;
		etherlinkerBatchResponseData.data = "Unknown network error in processing HTTP response.";

		if (!Request->GetHeader("UserIndex").IsEmpty())
		{
			etherlinkerBatchResponseData.userIndex = Request->GetHeader("UserIndex");
			UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
		}

		if (!Request->GetHeader("SenderId").IsEmpty())
		{
			etherlinkerBatchResponseData.senderId = Request->GetHeader("SenderId");
			UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
		}
		else {
			etherlinkerBatchResponseData.senderId = "DefaultErrorProcessor";
			UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
		}

		OnBatchResponseReceivedEvent.Broadcast("error", etherlinkerBatchResponseData);
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
				FEtherlinkerBatchResponseData etherlinkerBatchResponseData;
				etherlinkerBatchResponseData.data = errorText;

				if (!Request->GetHeader("UserIndex").IsEmpty())
				{
					etherlinkerBatchResponseData.userIndex = Request->GetHeader("UserIndex");
					UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
				}

				if (!Request->GetHeader("SenderId").IsEmpty())
				{
					etherlinkerBatchResponseData.senderId = Request->GetHeader("SenderId");
					UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
				}
				else {
					etherlinkerBatchResponseData.senderId = "DefaultErrorProcessor";
					UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
				}

				UE_LOG(LogTemp, Error, TEXT("%s"), *etherlinkerBatchResponseData.data);

				OnBatchResponseReceivedEvent.Broadcast(result, etherlinkerBatchResponseData);
			}
			else {
				// Process normal response
				TSharedPtr<FJsonObject> JsonResponseObject = JsonObject->GetObjectField("data");
				FEtherlinkerBatchResponseData etherlinkerBatchResponseData;
				FJsonObjectConverter::JsonAttributesToUStruct(JsonResponseObject->Values, FEtherlinkerBatchResponseData::StaticStruct(), &etherlinkerBatchResponseData, 0, 0);

#if !UE_BUILD_SHIPPING
				UE_LOG(LogTemp, Warning, TEXT("%s"), *etherlinkerBatchResponseData.data);
#endif

				OnBatchResponseReceivedEvent.Broadcast(result, etherlinkerBatchResponseData);
			}

		}
		else {
			// If we can't deserialize JSON response
			FEtherlinkerBatchResponseData etherlinkerBatchResponseData;
			etherlinkerBatchResponseData.data = "Can't deserialize JSON response.";

			if (!Request->GetHeader("UserIndex").IsEmpty())
			{
				etherlinkerBatchResponseData.userIndex = Request->GetHeader("UserIndex");
				UE_LOG(LogTemp, Error, TEXT("User Index: %s"), *Request->GetHeader("UserIndex"));
			}

			if (!Request->GetHeader("SenderId").IsEmpty())
			{
				etherlinkerBatchResponseData.senderId = Request->GetHeader("SenderId");
				UE_LOG(LogTemp, Error, TEXT("Sender Id: %s"), *Request->GetHeader("SenderId"));
			}
			else {
				etherlinkerBatchResponseData.senderId = "DefaultErrorProcessor";
				UE_LOG(LogTemp, Error, TEXT("Sender Id: DefaultErrorProcessor"));
			}

			OnBatchResponseReceivedEvent.Broadcast("error", etherlinkerBatchResponseData);
			UE_LOG(LogTemp, Error, TEXT("Can't deserialize JSON response."));
		}
	}
}
