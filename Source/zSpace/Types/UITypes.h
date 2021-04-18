// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Engine/DataAsset.h>
#include <UObject/Object.h>
#include "UITypes.generated.h"


UENUM(BlueprintType)
enum class EResolution : uint8
{
	None,

	R_5120X1440		UMETA(DisplayName="5120X1440"),
	R_1920X1080		UMETA(DisplayName="1920X1080"),
	R_1440X900		UMETA(DisplayName="1440X900"),
};

UCLASS(BlueprintType)
class ZSPACE_API UConvertEResolutionToFIntPointOrViceVersa : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContext"))
	static FIntPoint GetIntPoint(const UObject* WorldContext, EResolution Resolution);

	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContext"))
	static EResolution GetEnumResolution(const UObject* WorldContext, FIntPoint Resolution);
};

USTRUCT(BlueprintType)
struct ZSPACE_API FResolutionAndWidgetSturct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EResolution, TSubclassOf<class UUserWidget>> ResolutionAndWidget;
};

UCLASS(BlueprintType)
class ZSPACE_API UResolutionAndWidgetDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FResolutionAndWidgetSturct ResolutionAndWidget;

	UFUNCTION(BlueprintPure)
	TSubclassOf<class UUserWidget> GetWidget(EResolution ResolutionEnum);
};

UCLASS(BlueprintType)
class ZSPACE_API UUseResolutionInGame : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EResolution> Resolutions;
};

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	PreLogin,
	Login,
	Register,
	Gameplay,
	SelectCharacter,
};

USTRUCT(BlueprintType)
struct ZSPACE_API FCharacterSelectBoxInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite)
	FString CharacterLevel;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* BackgroundImage = nullptr;
};

USTRUCT(BlueprintType)
struct ZSPACE_API FBackgroundVideo
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaPlayer* MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaSource* MediaSource = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* VideoMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* MenuSound = nullptr;
};


