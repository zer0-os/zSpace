// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "CopySetupInfo.generated.h"

/**
 * 
 */
UCLASS()
class SETUPCOPY_API UCopySetupInfo final : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UCopySetupInfo();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Data Asset")
	void CreateFileMaterialInfo(const FString& CarName);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Data Asset")
	void SetMaterialsByFile();
	
	static FName GetJsonDir();
};