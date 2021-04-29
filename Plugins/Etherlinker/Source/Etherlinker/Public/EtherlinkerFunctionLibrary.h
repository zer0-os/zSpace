// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "JsonGlobals.h"
#include "Policies/JsonPrintPolicy.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonTypes.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JsonObjectConverter.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Components/WidgetInteractionComponent.h"
#include "EtherlinkerTypes.h"
#include "EtherlinkerFunctionLibrary.generated.h"

UCLASS()
class ETHERLINKER_API UEtherlinkerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
 
public:

	/**
	 * Start integration server
	 *
	 * @return true if integration server started successfully
	 */
	UFUNCTION(BlueprintCallable, Category="Etherlinker")
	static bool StartIntegrationServer();
	
	/**
	 * Stop integration server
	 *
	 * @return true if integration server stopped successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool StopIntegrationServer();

	/**
	 * Restart integration server
	 *
	 * @return true if integration server restarted successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool RestartIntegrationServer();

	/**
	 * Check integration server
	 *
	 * @return true if integration server currently running
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool CheckIntegrationServer();

	/**
	 * Compile custom solidity contracts from specified directory
	 *
	 * @return true if contracts and integration server compiled successfully and integration server started successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool CompileContracts();

	/**
	 * Compile integration server source code
	 *
	 * @return true if integration server compiled and started successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool CompileIntegrationServer();

	/**
	 * Get wallet address from keystore file with private key
	 *
	 * @param FilePath path to the keystore file with private key
	 * @return Wallet address or empty string
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static FString GetWalletAddressFromKeystoreFile(const FString& FilePath);

	/**
	 * Save wallet to selected file
	 *
	 * @param FilePath full path with file name
	 * @param Password encryption password
	 * @param WalletData wallet data input
	 * @return true if operation successfully completed
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool SaveWalletToFile(const FString& FilePath, const FString& Password, FWalletData WalletData);

	/**
	 * Load wallet from selected file
	 *
	 * @param FilePath full path with file name
	 * @param Password decryption password
	 * @param WalletData wallet data output
	 * @return true if operation successfully completed
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static bool LoadWalletFromFile(const FString& FilePath, const FString& Password, FWalletData& WalletData);

	/**
	 * Encrypt specified string via AES-256 encryption algorithm
	 *
	 * @param InputString input string to encrypt
	 * @param Key password as an encryption key
	 * @return String encrypted string
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static FString Encrypt(FString InputString, FString Key);

	/**
	 * Decrypt specified string via AES-256 encryption algorithm
	 *
	 * @param InputString input string to decrypt
	 * @param Key password as an encryption key
	 * @return String decrypted string
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker")
	static FString Decrypt(FString InputString, FString Key);

	/**
	 * Check if HMD connected or not
	 *
	 * @return true if VR headset connected
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static bool CheckHMDConnectionStatus();

	/**
	 * Check if we're in the VR preview mode via UE4 Editor or in a packaged game
	 *
	 * @return true if we're in the VR preview mode via UE4 Editor or in a packaged game
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static bool CheckVRPreviewOrGameActive();

	/**
	 * Get HMD Pure Yaw
	 *
	 * @return HMD Pure Yaw
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static FRotator GetHMDPureYaw(FRotator HMDRotation);

	/**
	 * Get Impact Point for 3D Widget
	 *
	 * @param Widget Widget to interact
	 * @param WidgetInteractor Widget Interactor
	 * @param ImpactPoint Default Impact Point from line trace method
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static FVector GetImpactPointForWidget(UWidgetComponent *Widget, UWidgetInteractionComponent *WidgetInteractor, FVector ImpactPoint);

	/**
	 * Validate wallet address
	 * @param WalletAddress wallet address
	 *
	 * @return true if wallet address has a valid format (0x...)
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static bool IsWalletAddressValid(FString WalletAddress);

	/**
	 * Check if file exists
	 * @param Filename filename
	 *
	 * @return true if file exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Etherlinker VR")
	static bool FileExists(FString Filename);

	static FProcHandle ProcessHandle;

};