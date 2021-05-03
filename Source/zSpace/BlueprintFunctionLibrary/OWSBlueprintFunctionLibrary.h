// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWSBlueprintFunctionLibrary.generated.h"


/** MeshName_CharacterName */
#define MESH_NAME FString("MeshName")

UENUM(BlueprintType)
enum class ECustomCharacterDataType : uint8
{
	MeshName,	
};

/**
 * 
 */
UCLASS()
class ZSPACE_API UOWSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContext"))
	static FString GetMeshFieldName(const UObject* WorldContext, const FString& CharacterName) { return MESH_NAME + "_" + CharacterName; }
	
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContext"))
	static FString GetCharacterNameFromMeshFieldName(const UObject* WorldContext, const FString& MeshFieldName);
};
