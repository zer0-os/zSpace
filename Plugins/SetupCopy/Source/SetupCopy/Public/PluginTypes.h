// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PluginTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMaterialInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> MaterialsPath;
};

USTRUCT(BlueprintType)
struct FMaterialInfoArray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMaterialInfo> MaterialInfos;
};
