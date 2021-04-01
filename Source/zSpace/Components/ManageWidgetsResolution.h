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

protected:
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> PreLoginWidgets;
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> LoginWidgets;

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	bool CreateWidgetAndAddViewprot(class APlayerController* PlayerControler, TSubclassOf<class UUserWidget> WidgetSubClass, EResolution Resolution, UUserWidget*& ReturnWidget);

	UFUNCTION(BlueprintPure, Category="UI")
	TMap<EResolution, class UUserWidget*>& GetWidgetsByEnum(EWidgetType WidgetType, bool& Result);

	UFUNCTION(BlueprintPure)
	class UUserWidget* GetWidgetByResolution(EWidgetType WidgetType, EResolution Resolution);

private:
	UPROPERTY()
	TMap<EResolution, class UUserWidget*> EmptyWidgets;
		
};
