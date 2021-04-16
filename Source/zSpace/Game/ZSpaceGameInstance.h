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

	UFUNCTION(BlueprintPure)
	class UManageWidgetsResolution* GetManageWidgetsResolution() const;

protected:
	
	virtual void Init() override;

public:
	
	UFUNCTION(BlueprintCallable)
	void SetNextMapPortal();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UManageWidgetsResolution> ManageWidgetsResolutionSubClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	FTimerHandle TimeHandleNextMapPortal;


	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	uint8 bNextMapPortal:1;
	
	
};
