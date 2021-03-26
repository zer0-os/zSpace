// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterUnderFootSurfaceDA.generated.h"

USTRUCT(BlueprintType)
struct FCharacterUnderFootSurfaceData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	UPhysicalMaterial *  PhysicalMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<class USoundBase *> SurfaceSoundBaseArray;
	
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

public:

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterUnderFootSurfaceData> GetCharacterUnderFootSurfaceDataArray() const;

	UFUNCTION(BlueprintCallable)
	FCharacterUnderFootSurfaceData GetCharacterUnderFootSurfaceDataByPhysicsType(const class UPhysicalMaterial * NewPhysicalMaterial, bool & NewIsValid);
	
};
