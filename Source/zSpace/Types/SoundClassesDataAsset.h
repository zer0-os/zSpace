// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundClassesDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ZSPACE_API USoundClassesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftObjectPtr<class USoundClass>> SoundClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundMix* GlobalSoundMix;

	UFUNCTION(BlueprintCallable)
	class USoundClass* GetSoundClassByName(FName SoundClassName);
	
};
