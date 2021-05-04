// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UITypes.generated.h"


UENUM(BlueprintType)
enum class EResolution : uint8
{
	None,

	R_5120X1440		UMETA(DisplayName="5120X1440"),
	R_1920X1080		UMETA(DisplayName="1920X1080"),
	R_1440X900		UMETA(DisplayName="1440X900"),

	MAX				UMETA(Hidden),
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
struct ZSPACE_API FResolutionAndWidgetStruct
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
	FResolutionAndWidgetStruct ResolutionAndWidget;

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
enum class EResolutionButtonStyleType : uint8
{
	NormalStyle,
	ActiveStyle
};

UCLASS(BlueprintType)
class ZSPACE_API UButtonStyleByResolution : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EResolution, FButtonStyle> NormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EResolution, FButtonStyle> ActiveStyle;

	UFUNCTION(BlueprintPure, BlueprintCosmetic, meta = (WorldContext="WorldContext"))
	static FButtonStyle GetButtonStyleByResolution(const class UObject* WorldContext, EResolution Resolution, EResolutionButtonStyleType ResolutionButtonStyle, UButtonStyleByResolution* ButtonStyleByResolution);
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

UENUM(BlueprintType)
enum class EPreviewCharacterPosition : uint8
{
	L_1,
	L_2,
	L_3,
	L_4,
	L_5,
	Middle,
	R_1,
	R_2,
	R_3,
	R_4,
	R_5,
};

USTRUCT(BlueprintType)
struct ZSPACE_API FCharacterInfoForUI
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite)
	FString CharacterLevel;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* BackgroundImage = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	EPreviewCharacterPosition PreviewCharacterDirection;
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

UCLASS(BlueprintType)
class ZSPACE_API URenderTargetAndPosition : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EPreviewCharacterPosition, UTextureRenderTarget2D*> RenderTargetAndPosition;
	
};


