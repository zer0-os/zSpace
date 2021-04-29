// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "EtherlinkerFunctionLibrary.h"
#include "EtherlinkerSettings.h"
#include "Engine/Engine.h"
#include "Runtime/HeadMountedDisplay/Public/IXRTrackingSystem.h"
#include "Runtime/HeadMountedDisplay/Public/IHeadMountedDisplay.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/HAL/Filemanager.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/Core/Public/Misc/AES.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/Internationalization/Regex.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#endif

FProcHandle UEtherlinkerFunctionLibrary::ProcessHandle;

bool UEtherlinkerFunctionLibrary::StartIntegrationServer()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif
    
	if (UEtherlinkerFunctionLibrary::ProcessHandle.IsValid()) {
		if (FPlatformProcess::IsProcRunning(ProcessHandle)) {
			UE_LOG(LogTemp, Warning, TEXT("Integration server already started"));
			return false;
		}
		else {
			// If we have a valid ProcessHandle, but app is not running - clear it
			FPlatformProcess::CloseProc(UEtherlinkerFunctionLibrary::ProcessHandle);
		}
	}

	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);
	FString ContentPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	FString Command;

#if PLATFORM_WINDOWS
	if (EtherlinkerSettings->RunIntegrationServerSilently) {
		Command = "javaw";
	}
	else {
		Command = "java";
	}
#elif PLATFORM_MAC
	Command = "/usr/bin/java";
#endif

	FString Args = EtherlinkerSettings->IntegrationServerParameters.TrimStartAndEnd() + " -jar " + "\"" + ContentPath + "IntegrationServer/stratum-1.0.war\"";
	uint32 ProcessId = 0;

#if PLATFORM_WINDOWS
	if (EtherlinkerSettings->RunIntegrationServerSilently) {
		UEtherlinkerFunctionLibrary::ProcessHandle = FPlatformProcess::CreateProc(*Command, *Args, false, false, true, &ProcessId, 0, nullptr, nullptr);
	}
	else {
		UEtherlinkerFunctionLibrary::ProcessHandle = FPlatformProcess::CreateProc(*Command, *Args, false, true, false, &ProcessId, 0, nullptr, nullptr);
	}
#elif PLATFORM_MAC || PLATFORM_LINUX
	UEtherlinkerFunctionLibrary::ProcessHandle = FPlatformProcess::CreateProc(*Command, *Args, false, true, false, &ProcessId, 0, nullptr, nullptr);
#endif

	UE_LOG(LogTemp, Warning, TEXT("Args: %s"), *Args);
	UE_LOG(LogTemp, Warning, TEXT("Process Id: %d"), ProcessId);
	UE_LOG(LogTemp, Warning, TEXT("Integration server started"));

	return true;
}

bool UEtherlinkerFunctionLibrary::StopIntegrationServer()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif
    
	if (UEtherlinkerFunctionLibrary::ProcessHandle.IsValid())
	{
		if (FPlatformProcess::IsProcRunning(ProcessHandle)) {
			FPlatformProcess::TerminateProc(UEtherlinkerFunctionLibrary::ProcessHandle);
			FPlatformProcess::CloseProc(UEtherlinkerFunctionLibrary::ProcessHandle);
			UE_LOG(LogTemp, Warning, TEXT("Integration server stopped"));
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Integration server is not started"));
	return false;
}

bool UEtherlinkerFunctionLibrary::RestartIntegrationServer()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif
    
	// If we have a valid ProcessHandle reference
	if (UEtherlinkerFunctionLibrary::ProcessHandle.IsValid())
	{
		// Check if server still running
		if (FPlatformProcess::IsProcRunning(ProcessHandle)) {
			// Trying to stop server
			if (StopIntegrationServer()) {
				// Trying to start server
				if (StartIntegrationServer()) {
					UE_LOG(LogTemp, Warning, TEXT("Integration server successfully restarted"));
					return true;
				}
			}
			// If we can't stop or start server, return false
			UE_LOG(LogTemp, Warning, TEXT("Can't restart integration server"));
			return false;
		}
		else {
			// If server is not running, but we have a ProcessHandle reference - clear reference and try to start server
			FPlatformProcess::CloseProc(UEtherlinkerFunctionLibrary::ProcessHandle);
			if (StartIntegrationServer()) {
				UE_LOG(LogTemp, Warning, TEXT("Integration server successfully restarted"));
				return true;
			}
			// If we failed to start server - return false
			UE_LOG(LogTemp, Warning, TEXT("Can't restart integration server"));
			return false;
		}
		
	}
	else {
		// If we don't have a ProcessHandle reference - trying to start server
		if (StartIntegrationServer()) {
			UE_LOG(LogTemp, Warning, TEXT("Integration server successfully restarted"));
			return true;
		}
		else {
			// Return false if we can't start server
			UE_LOG(LogTemp, Warning, TEXT("Can't restart integration server"));
			return false;
		}
	}
}

bool UEtherlinkerFunctionLibrary::CheckIntegrationServer()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif
    
	if (UEtherlinkerFunctionLibrary::ProcessHandle.IsValid())
	{
		if (FPlatformProcess::IsProcRunning(ProcessHandle)) {
			UE_LOG(LogTemp, Warning, TEXT("Integration server is running"));
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Integration server is not running"));
			return false;
		}
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Integration server is not running"));
		return false;
	}
}

bool UEtherlinkerFunctionLibrary::CompileContracts()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif

	// Step 1: Search for contract files
	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);

	FString ContractsDir = EtherlinkerSettings->ContractsToCompileDirectory;
	FPaths::NormalizeDirectoryName(ContractsDir);

	FString IntegrationServerSourceDirectory = EtherlinkerSettings->IntegrationServerSourceDirectory;
	FPaths::NormalizeDirectoryName(IntegrationServerSourceDirectory);

	// Get contracts destination directory
	FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	//FString ContractsDestinationDirectory = ProjectDir.Left(ProjectDir.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, ProjectDir.Len()-1)) + "/EtherlinkerServer/Stratum/src/main/resources/solidity";
	FString ContractsDestinationDirectory = IntegrationServerSourceDirectory + "/src/main/resources/solidity";

	UE_LOG(LogTemp, Warning, TEXT("ContractsDestinationDirectory: %s"), *ContractsDestinationDirectory);
	
	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> ContractFiles;
	FString ContractsSourceDirectory = ContractsDir + "/" + "*.sol";
	UE_LOG(LogTemp, Warning, TEXT("ContractsSourceDirectory: %s"), *ContractsSourceDirectory);
	FileManager.FindFiles(ContractFiles, *ContractsSourceDirectory, true, false);
	
// 	if (ContractFiles.Num() == 0) {
// 		UE_LOG(LogTemp, Warning, TEXT("Could not find any contracts to compile"));
// 		return false;
// 	}

	// Step 2: Copy contract files to integration server source folder
	for (FString ContractFile : ContractFiles)
	{

		FString ContractFilePath = ContractsDir + "/" + ContractFile;
		UE_LOG(LogTemp, Warning, TEXT("ContractSourceFilePath: %s"), *ContractFilePath);
		UE_LOG(LogTemp, Warning, TEXT("ContractDestinationFilePath: %s"), *(ContractsDestinationDirectory + "/" + ContractFile));

		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ContractFilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find file %s"), *ContractFilePath);
			return false;
		}

		if (!FPlatformFileManager::Get().GetPlatformFile().MoveFile(*(ContractsDestinationDirectory + "/" + ContractFile), *ContractFilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not move file %s"), *ContractFilePath);
			return false;
		}
	}
	
	// Step 3: Compile contracts with Web3j

	FString Command;

#if PLATFORM_WINDOWS
	Command = "\"\"" + ProjectDir + "Content/IntegrationServer/" + EtherlinkerSettings->CompileContractsScript + "\" \"" + IntegrationServerSourceDirectory + "\"\"";
#elif PLATFORM_MAC || PLATFORM_LINUX
	Command = "bash \"" + ProjectDir + "Content/IntegrationServer/" + EtherlinkerSettings->CompileContractsScript + "\" \"" + IntegrationServerSourceDirectory + "\"";
#endif

	UE_LOG(LogTemp, Warning, TEXT("Compile contracts command: %s"), *Command)

	auto Conv = StringCast<ANSICHAR>(*Command);
	const ANSICHAR* Ptr = Conv.Get();
    
#if !(PLATFORM_IOS || PLATFORM_ANDROID)
	int ReturnCode = system(Ptr);
    
	if (ReturnCode == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Contract compilation completed successfully"));
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Contract compilation failed with error code: %d"), ReturnCode);
		return false;
	}
    
#endif
	
}

bool UEtherlinkerFunctionLibrary::CompileIntegrationServer()
{
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    return false;
#endif

	// Step 1: Compile IntegrationServer
	if (!StopIntegrationServer()) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to stop integration server"));
	}
	UEtherlinkerSettings* EtherlinkerSettings = GetMutableDefault<UEtherlinkerSettings>();
	check(EtherlinkerSettings);

	FString IntegrationServerSourceDirectory = EtherlinkerSettings->IntegrationServerSourceDirectory;
	FPaths::NormalizeDirectoryName(IntegrationServerSourceDirectory);

	FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FString Command;

#if PLATFORM_WINDOWS
	Command = "\"\"" + ProjectDir + "Content/IntegrationServer/" + EtherlinkerSettings->CompileIntegrationServerScript + "\" \"" + IntegrationServerSourceDirectory + "\"\"";
#elif PLATFORM_MAC || PLATFORM_LINUX
	Command = "bash \"" + ProjectDir + "Content/IntegrationServer/" + EtherlinkerSettings->CompileIntegrationServerScript + "\" \"" + IntegrationServerSourceDirectory + "\"";
#endif

	UE_LOG(LogTemp, Warning, TEXT("Compile integration server command: %s"), *Command);

	auto Conv = StringCast<ANSICHAR>(*Command);
	const ANSICHAR* Ptr = Conv.Get();
    
#if !(PLATFORM_IOS || PLATFORM_ANDROID)
	int ReturnCode = system(Ptr);

	if (ReturnCode == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Integration server compilation completed successfully"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Integration server compilation failed with error code: %d"), ReturnCode);
		return false;
	}
#endif
    
	// Step 2: Copy new .war file to the Content/IntegrationServer
	FString WarFileSourcePath = IntegrationServerSourceDirectory + "/target/stratum-1.0.war";
	FString WarFileDestinationPath = ProjectDir + "Content/IntegrationServer/stratum-1.0.war";
	
	UE_LOG(LogTemp, Warning, TEXT("WarFileSourcePath: %s"), *WarFileSourcePath);
	UE_LOG(LogTemp, Warning, TEXT("WarFileDestinationPath: %s"), *WarFileDestinationPath);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*WarFileSourcePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find new .war file %s"), *WarFileSourcePath);
		return false;
	}

	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*WarFileDestinationPath))
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*WarFileDestinationPath)) {
			UE_LOG(LogTemp, Warning, TEXT("Could not delete old .war file %s"), *WarFileDestinationPath);
			return false;
		}
	}

	if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*WarFileDestinationPath, *WarFileSourcePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not copy file %s"), *WarFileSourcePath);
		return false;
	}

	// Step 3: Restart integration server
	if (!StartIntegrationServer()) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to start integration server"));
	}
	
	return true;
}

FString UEtherlinkerFunctionLibrary::GetWalletAddressFromKeystoreFile(const FString& FilePath)
{
	if (FilePath.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("GetWalletAddressFromKeystoreFile method error: file path is empty"));
		return "";
	}

	FString JSONString;
	FFileHelper::LoadFileToString(JSONString, *FilePath);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JSONString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		return "0x" + JsonObject->GetStringField("address");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWalletAddressFromKeystoreFile method error: can't read wallet address from keystore file"));
		return "";
	}
}

bool UEtherlinkerFunctionLibrary::SaveWalletToFile(const FString& FilePath, const FString& Password, FWalletData WalletData)
{

	// Encrypt data
	WalletData.walletAddress = Encrypt(WalletData.walletAddress, Password);
	WalletData.walletPassword = Encrypt(WalletData.walletPassword, Password);
	WalletData.walletMnemonic = Encrypt(WalletData.walletMnemonic, Password);
	WalletData.walletPath = Encrypt(WalletData.walletPath, Password);
	WalletData.walletAuthType = Encrypt(WalletData.walletAuthType, Password);

	if (WalletData.walletAddress.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("SaveWalletToFile: encryption error during wallet saving"));
		return false;
	}

	FBufferArchive ToBinary;

	UStruct* WalletDataStruct = FWalletData::StaticStruct();
	WalletDataStruct->SerializeBin(ToBinary, &WalletData);

	if (ToBinary.Num() <= 0) return false;

	FText errorText;
	if (!FFileHelper::IsFilenameValidForSaving(FilePath, errorText)) {
		UE_LOG(LogTemp, Warning, TEXT("SaveWalletToFile error: invalid filename"));
		return false;
	}

	if (FPaths::FileExists(FilePath)) {
		UE_LOG(LogTemp, Warning, TEXT("SaveWalletToFile error: file already exists"));
		return false;
	}

	if (FFileHelper::SaveArrayToFile(ToBinary, *FilePath))
	{			
		ToBinary.FlushCache();
		ToBinary.Empty();

		UE_LOG(LogTemp, Warning, TEXT("SaveWalletToFile: wallet successfully saved"));
		return true;
	}
	else {
		ToBinary.FlushCache();
		ToBinary.Empty();
		UE_LOG(LogTemp, Warning, TEXT("SaveWalletToFile: error during wallet saving"));
		return false;
	}
}

bool UEtherlinkerFunctionLibrary::LoadWalletFromFile(const FString& FilePath, const FString& Password, FWalletData& WalletData)
{

	FText errorText;
	if (!FFileHelper::IsFilenameValidForSaving(FilePath, errorText)) {
		UE_LOG(LogTemp, Warning, TEXT("LoadWalletFromFile error: invalid filename"));
		return false;
	}

	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadWalletFromFile: error during wallet loading"));
		return false;
	}

	if (TheBinaryArray.Num() <= 0) return false;

	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); 
	FromBinary.Seek(0);

	UStruct* WalletDataStruct = FWalletData::StaticStruct();
	WalletDataStruct->SerializeBin(FromBinary, &WalletData);

	WalletData.walletAddress = Decrypt(WalletData.walletAddress, Password);	
	WalletData.walletPassword = Decrypt(WalletData.walletPassword, Password);
	WalletData.walletMnemonic = Decrypt(WalletData.walletMnemonic, Password);
	WalletData.walletPath = Decrypt(WalletData.walletPath, Password);
	WalletData.walletAuthType = Decrypt(WalletData.walletAuthType, Password);

	if (WalletData.walletAddress.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("LoadWalletFromFile: decryption error during wallet loading"));
		return false;
	}

	FromBinary.FlushCache();
	TheBinaryArray.Empty();
	FromBinary.Close();

	UE_LOG(LogTemp, Warning, TEXT("LoadWalletFromFile: wallet successfully loaded"));

	return true;

}

FString UEtherlinkerFunctionLibrary::Encrypt(FString InputString, FString Key)
{
	// Check inputs
	if (InputString.IsEmpty()) return "";  //empty string? do nothing
	if (Key.IsEmpty()) return "";

	// To split correctly final result of decryption from trash symbols
	FString SplitSymbol = "EL@$@!";
	InputString.Append(SplitSymbol);

	// We need at least 32 symbols key
	Key = FMD5::HashAnsiString(*Key);
	TCHAR *KeyTChar = Key.GetCharArray().GetData();           
	ANSICHAR *KeyAnsi = (ANSICHAR*)TCHAR_TO_ANSI(KeyTChar);

	// Calculate blob size and create blob
	uint8* Blob; 
	uint32 Size; 
	 
	Size = InputString.Len();
	Size = Size + (FAES::AESBlockSize - (Size % FAES::AESBlockSize));

	Blob = new uint8[Size];

	// Convert string to bytes and encrypt
	if(StringToBytes(InputString, Blob, Size)) {

		FAES::EncryptData(Blob, Size, KeyAnsi);
		InputString = FString::FromHexBlob(Blob, Size);

		delete Blob;
		return InputString;		
	}

	delete Blob;
	return ""; 
}

FString UEtherlinkerFunctionLibrary::Decrypt(FString InputString, FString Key)
{
	// Check inputs
	if (InputString.IsEmpty()) return ""; 
	if (Key.IsEmpty()) return "";

	// To split correctly final result of decryption from trash symbols
	FString SplitSymbol = "EL@$@!";

	// We need at least 32 symbols key
	Key = FMD5::HashAnsiString(*Key);
	TCHAR *KeyTChar = Key.GetCharArray().GetData();
	ANSICHAR *KeyAnsi = (ANSICHAR*)TCHAR_TO_ANSI(KeyTChar);
	
	// Calculate blob size and create blob
	uint8* Blob; 
	uint32 Size; 

	Size = InputString.Len();
	Size = Size + (FAES::AESBlockSize - (Size % FAES::AESBlockSize));

	Blob = new uint8[Size]; 

	// Convert string to bytes and decrypt
	if (FString::ToHexBlob(InputString, Blob, Size)) {

		FAES::DecryptData(Blob, Size, KeyAnsi);		
		InputString = BytesToString(Blob, Size);

		// Split required data from trash
		FString LeftData;	
		FString RightData;	
		InputString.Split(SplitSymbol, &LeftData, &RightData, ESearchCase::CaseSensitive, ESearchDir::FromStart);
		InputString = LeftData;

		delete Blob; 
		return InputString; 
	}

	delete Blob; 
	return ""; 
}

bool UEtherlinkerFunctionLibrary::CheckHMDConnectionStatus()
{
	return GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetHMDDevice() && GEngine->XRSystem->GetHMDDevice()->IsHMDConnected();
}

bool UEtherlinkerFunctionLibrary::CheckVRPreviewOrGameActive()
{

#if WITH_EDITOR
	if (GIsEditor)
	{
		if (UEditorEngine* EdEngine = Cast<UEditorEngine>(GEngine))
		{
			TOptional<FPlayInEditorSessionInfo> PlayInfo = EdEngine->GetPlayInEditorSessionInfo();
			if (PlayInfo.IsSet())
			{
				return PlayInfo->OriginalRequestParams.SessionPreviewTypeOverride == EPlaySessionPreviewType::VRPreview;
			}
			else
			{
				return false;
			}
		}
	}
#endif

	return true;
}

FRotator UEtherlinkerFunctionLibrary::GetHMDPureYaw(FRotator HMDRotation)
{
	FRotationMatrix HeadMat(HMDRotation);
	FVector forward = HeadMat.GetScaledAxis(EAxis::X);
	FVector forwardLeveled = forward;
	forwardLeveled.Z = 0;
	forwardLeveled.Normalize();
	FVector mixedInLocalForward = HeadMat.GetScaledAxis(EAxis::Z);

	if (forward.Z > 0)
	{
		mixedInLocalForward = -mixedInLocalForward;
	}

	mixedInLocalForward.Z = 0;
	mixedInLocalForward.Normalize();
	float dot = FMath::Clamp(FVector::DotProduct(forwardLeveled, forward), 0.0f, 1.0f);
	FVector finalForward = FMath::Lerp(mixedInLocalForward, forwardLeveled, dot * dot);

	return FRotationMatrix::MakeFromXZ(finalForward, FVector::UpVector).Rotator();
}

FVector UEtherlinkerFunctionLibrary::GetImpactPointForWidget(UWidgetComponent *Widget, UWidgetInteractionComponent *WidgetInteractor, FVector ImpactPoint)
{

	if (Widget->GetGeometryMode() == EWidgetGeometryMode::Cylinder) {

		const FTransform WorldTransform = WidgetInteractor->GetComponentTransform();
		const FVector Direction = WorldTransform.GetUnitAxis(EAxis::X);

		TTuple<FVector, FVector2D> CylinderHitLocation = Widget->GetCylinderHitLocation(ImpactPoint, Direction);
		return CylinderHitLocation.Get<0>();
	}
	else {
		return ImpactPoint;
	}
}

bool UEtherlinkerFunctionLibrary::IsWalletAddressValid(FString WalletAddress)
{

	if (WalletAddress.IsEmpty()) {
		return false;
	}

	const FRegexPattern simpleAddressCheckPattern(TEXT("^(0x)[0-9a-fA-F]{40}$"));
	FRegexMatcher simpleAddressCheckMatcher(simpleAddressCheckPattern, WalletAddress);

	return simpleAddressCheckMatcher.FindNext();
}

bool UEtherlinkerFunctionLibrary::FileExists(FString Filename)
{

	if (Filename.IsEmpty()) {
		return false;
	}

	return FPaths::FileExists(Filename);
}
