// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "zSpace/Types/UITypes.h"
#include "ManageWidgetsResolution.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ZSPACE_API UManageWidgetsResolution : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManageWidgetsResolution();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetChanged, class UUserWidget*, Widget, EWidgetType, WidgetType);
	UPROPERTY(BlueprintAssignable)
	FOnWidgetChanged OnWidgetChanged;

protected:
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> PreLoginWidgets;
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> LoginWidgets;
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> RegisterWidgets;
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> GameplayWidgets;
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> SelectCharacterWidgets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWidgetsDataAsset WidgetsDataAsset;

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	bool CreateWidgetAndAddViewport(class APlayerController* PlayerControler, TSubclassOf<class UUserWidget> WidgetSubClass, EResolution Resolution, UUserWidget*& ReturnWidget);

	UFUNCTION(BlueprintPure, Category="UI")
	TMap<EResolution, class UUserWidget*>& GetWidgetsByEnum(EWidgetType WidgetType, bool& Result);

	UFUNCTION(BlueprintPure, Category="UI")
	class UUserWidget* GetWidgetByResolution(EWidgetType WidgetType, EResolution Resolution);

	UFUNCTION(BlueprintCallable, Category="UI")
	void RemoveFromParentWidgets(TMap<EResolution, class UUserWidget*>& Widgets);

	UFUNCTION(BlueprintGetter, BlueprintCosmetic)
	bool GetIsGameplayWidgetHidden() const;

	UFUNCTION(BlueprintSetter, BlueprintCosmetic)
	void SetIsGameplayWidgetHidden(bool NewValue);

	UFUNCTION(BlueprintPure)
	UResolutionAndWidgetDataAsset* GetWidgetDataAssetByWidgetType(const EWidgetType& WidgetType) const;
	
protected:
	UPROPERTY(BlueprintReadWrite, BlueprintGetter=GetIsGameplayWidgetHidden, BlueprintSetter=SetIsGameplayWidgetHidden)
	uint8 bIsGameplayWidgetHidden : 1;
	
private:
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> EmptyWidgets;
		
};
