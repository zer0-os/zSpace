// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetLoadingManagerObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ZSPACE_API UWidgetLoadingManagerObject : public UObject
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * UserWidgetLoading = nullptr;
	
public:

	UFUNCTION(BlueprintCallable, meta = (NewZOrder=99))
	void ShowLoadingWidget(TSubclassOf<class UUserWidget> NewLoadingWidget, int32 NewZOrder);

	UFUNCTION(BlueprintCallable)
	void HideLoadingWidget();
	
};
