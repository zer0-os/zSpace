// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PortalSoundsDataAsset.generated.h"

/**
 * 
 */

 UENUM(BlueprintType)
enum class EPortalSoundType : uint8
{
	FadeIn UMETA(DisplayName = "FadeIn"),
	FadeOut UMETA(DisplayName = "FadeOut"),
	MidLoop UMETA(DisplayName = "MidLoop"),
};

UCLASS()
class ZSPACE_API UPortalSoundsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* PortalFadeInSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* PortalFadeOutSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* PortalMiddleSound;
	
};
