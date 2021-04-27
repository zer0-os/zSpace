// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "CharacterUnderFootSurfaceDA.generated.h"


USTRUCT(BlueprintType)
struct FFootHitGroundData
{
	GENERATED_BODY()

	FFootHitGroundData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> PreviousMovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class USoundBase> SoundBase;

	bool operator==(const FFootHitGroundData& NewFootHitGroundData);
	
	friend  bool operator==(EMovementMode& NewMovementMode, const FFootHitGroundData& NewFootHitGroundDataB);
	
	friend  bool operator==( const FFootHitGroundData& NewFootHitGroundDataA, EMovementMode& NewMovementModeB);
	
};


bool operator==(EMovementMode& NewMovementMode, const FFootHitGroundData& NewFootHitGroundDataB);

bool operator==( const FFootHitGroundData& NewFootHitGroundDataA, EMovementMode& NewMovementModeB);

USTRUCT(BlueprintType)
struct FCharacterUnderFootSurfaceData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	UPhysicalMaterial *  PhysicalMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<TSoftObjectPtr<class USoundBase >> SurfaceSoundBaseArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftObjectPtr<class UParticleSystem > SurfaceParticleSystem;
	
};

/**
 * TODO need to implement check functionality in Editor to check duplication of CharacterUnderFootSurfaceDataArray  member.
 */
UCLASS()
class ZSPACE_API UCharacterUnderFootSurfaceDA : public UDataAsset
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<FCharacterUnderFootSurfaceData> CharacterUnderFootSurfaceDataArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<FFootHitGroundData>	FootHitGroundDataArray;

public:

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterUnderFootSurfaceData> GetCharacterUnderFootSurfaceDataArray() const;

	UFUNCTION(BlueprintCallable)
	FCharacterUnderFootSurfaceData GetCharacterUnderFootSurfaceDataByPhysicsType(const class UPhysicalMaterial * NewPhysicalMaterial, bool & NewIsValid);

	UFUNCTION(BlueprintCallable)
	FFootHitGroundData GetFootHitGroundDataByMovementMode(EMovementMode MovementMode, bool & NewIsValid);
	
};
