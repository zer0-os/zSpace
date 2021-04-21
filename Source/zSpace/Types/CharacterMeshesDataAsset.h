// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterMeshesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UCharacterMeshesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USkeletalMesh*> CharacterMeshes;
	
};
