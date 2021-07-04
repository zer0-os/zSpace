// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ZSCameraComponent.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class ECameraPositionType : uint8
{
	 DefaultCamera UMETA(DisplayName = "Default Camera")
	,CameraInSide UMETA(DisplayName = "Camera in side")
	,CameraVRMode UMETA(DisplayName = "Camera VR Mode")
	,CPT_MAX      UMETA(Hidden)
	
};

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	ECameraPositionType GetCameraPositionType() const;

	UFUNCTION(BlueprintCallable)
	void SetCameraPositionType(ECameraPositionType NewCameraPositionType);

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	ECameraPositionType CameraPositionType;
	
};
