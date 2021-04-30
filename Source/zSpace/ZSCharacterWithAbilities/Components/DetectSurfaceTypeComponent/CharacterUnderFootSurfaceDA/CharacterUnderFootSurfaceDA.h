// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "CharacterUnderFootSurfaceDA.generated.h"

USTRUCT(BlueprintType)
struct FFootHitGroundEqualData
{
	GENERATED_BODY()
	
	FFootHitGroundEqualData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> PreviousMovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicalMaterial * PhysicalMaterial = nullptr;

	
};


USTRUCT(BlueprintType)
struct FFootHitGroundData
{
	GENERATED_BODY()

	FFootHitGroundData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> PreviousMovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> MovementMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	UPhysicalMaterial *  PhysicalMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class USoundBase> SoundBase;

	bool operator==(const FFootHitGroundData& NewFootHitGroundData);
	
	friend  bool operator==(FFootHitGroundEqualData & NewFootHitGroundEqualData, const FFootHitGroundData& NewFootHitGroundDataB);
	
	friend  bool operator==( const FFootHitGroundData& NewFootHitGroundDataA, FFootHitGroundEqualData & NewFootHitGroundEqualDataB);
	
};

bool operator==(FFootHitGroundEqualData & NewFootHitGroundEqualData, const FFootHitGroundData& NewFootHitGroundDataB);

bool operator==( const FFootHitGroundData& NewFootHitGroundDataA, FFootHitGroundEqualData & NewFootHitGroundEqualDataB);

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
	FFootHitGroundData GetFootHitGroundDataByMovementMode(EMovementMode NewPreviousMovementMode, EMovementMode NewCurrentMovementMode, UPhysicalMaterial * NewPhysicalMaterial, bool & NewIsValid);
	
};
