// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterMeshesDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FCharacterMeshData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh * CharacterMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsDefaultCharacter:1;
	
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ZSPACE_API UCharacterMeshesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<USkeletalMesh*, FName> CharacterMeshAndName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCharacterMeshData> CharacterMeshDataArray;

public:
	UFUNCTION(BlueprintPure)
	class USkeletalMesh* GetMeshByName(const FName& Name) const;
	
	UFUNCTION(BlueprintPure)
	FName GetNameByMesh(const USkeletalMesh* Mesh) const;

	UFUNCTION(BlueprintPure)
	TArray<class USkeletalMesh *> GetAllSkeletalMeshes();

	UFUNCTION(BlueprintPure)
	void GetAllCharacterNames(TArray<FName>& CharacterNames);

	UFUNCTION(BlueprintPure)
	void GetDefaultSkeletalMeshName(FString & NewDefaultSkeletalMeshName);
};
