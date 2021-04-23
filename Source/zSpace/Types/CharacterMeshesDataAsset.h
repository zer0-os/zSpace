// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterMeshesDataAsset.generated.h"

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

public:
	UFUNCTION(BlueprintPure)
	class USkeletalMesh* GetMeshByName(const FName& Name) const;
	
	UFUNCTION(BlueprintPure)
	FName GetNameByMesh(const USkeletalMesh* Mesh) const;
};
