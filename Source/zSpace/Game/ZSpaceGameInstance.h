// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSGameInstance.h"
#include "ZSpaceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSpaceGameInstance : public UOWSGameInstance
{
	GENERATED_BODY()

public:
	UZSpaceGameInstance(const FObjectInitializer& ObjectInitializer);

	// [Client]
	UFUNCTION(BlueprintCosmetic)
	void InitManageWidgetsResolution();

protected:
	virtual void Init() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UManageWidgetsResolution> ManageWidgetsResolutionSubClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;
};
