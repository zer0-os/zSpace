// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VehicleWIdgetsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FVehicleWidgets
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UUserWidget> UserWidgetControlHelperWidget;

	friend class UVehicleWidgetsDataAsset;
	
};

/**
 * 
 */
UCLASS()
class ZSPACE_API UVehicleWidgetsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FVehicleWidgets VehicleWidgets;
	
public:

	UFUNCTION(BlueprintPure)
	TSubclassOf<class UUserWidget> GetUserWidgetControlHelper() const;

	
};
