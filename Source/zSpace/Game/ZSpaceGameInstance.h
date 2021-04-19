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

	// [Client]
	UFUNCTION(BlueprintCosmetic)
	void CreateWidgetLoadingManagerObject();

protected:
	
	virtual void Init() override;

public:

	UFUNCTION(BlueprintPure)
	class UWidgetLoadingManagerObject * GetWidgetLoadingManagerObject() const ;
	

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UManageWidgetsResolution> ManageWidgetsResolutionSubClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	/**
	 * @brief The WidgetLoadingManagerObjectSubClass manager of loading widget.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UWidgetLoadingManagerObject> WidgetLoadingManagerObjectSubClass;
	
	/**
	 * @brief The WidgetLoadingManagerObject pointer of WidgetLoadingManagerObject
	 */
	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UWidgetLoadingManagerObject * WidgetLoadingManagerObject = nullptr;
	

	/**
	 * @brief The LoadingWidget is Widget 'sub class' that shows loading effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UUserWidget> LoadingWidgetSubClass;

};
