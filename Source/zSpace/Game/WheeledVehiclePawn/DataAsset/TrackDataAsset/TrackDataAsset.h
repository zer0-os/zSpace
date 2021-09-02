// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrackDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FTrackData
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PhysicalProperties, meta = (AllowPrivateAccess=true))
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UParticleSystem> ParticleSystemSoftClassPtr;

public:
	
	class  UParticleSystem * GetParticleSystem();

	friend class UTrackDataAsset;
};

/**
 * 
 */
UCLASS()
class ZSPACE_API UTrackDataAsset : public UDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<FTrackData> TrackDatas;
public:
	class  UParticleSystem * GetParticle(TEnumAsByte<EPhysicalSurface> CurrentSurface);
	
};
