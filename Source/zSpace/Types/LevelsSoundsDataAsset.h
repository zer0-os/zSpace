// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <UObject/NameTypes.h>
#include "LevelsSoundsDataAsset.generated.h"

/**
 * 
 */

 USTRUCT(BlueprintType)
 struct FLevelSoundsData 
 {

	GENERATED_BODY()
	
 public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class USoundBase>> Sounds; 

 };

UCLASS()
class ZSPACE_API ULevelsSoundsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FLevelSoundsData> LevelsSound;

public:

	UFUNCTION(BlueprintCallable)
	TArray<TSoftObjectPtr <class USoundBase>> GetSoundsByLevelName(FName LevelName);
	
};
