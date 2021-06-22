// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PortalSoundsDataAsset.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ZSPACE_API USoundManager : public UObject
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UPortalSoundsDataAsset* PortalSoundsDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USoundClassesDataAsset* SoundClassesDataAsset;

public:	

	UFUNCTION(BlueprintCallable)
	void PlayPortalSoundByType(EPortalSoundType SoundType);

	UFUNCTION(BlueprintCallable)
	void SetSoundVolumeBySoundClassName(FName SoundClassName, float Volume);

};
