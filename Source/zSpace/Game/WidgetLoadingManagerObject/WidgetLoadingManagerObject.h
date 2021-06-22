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

	 friend class UZSpaceGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UUserWidget> SecondLoadingWidget; 
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * UserWidgetLoading = nullptr;

	UPROPERTY()
	class UZSpaceGameInstance * ZSpaceGameInstance = nullptr;

	uint8 bNotShowLoadingWidget:1;

	uint8 bLastNotShowLoadingWidget:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	UTexture * TextureLoading = nullptr;
	
public:


	UFUNCTION(BlueprintCallable)
	void SetNotShowLoadingWidget(bool NewNotShowLoadingWidget);

	void SetZSpaceGameInstance(class UZSpaceGameInstance * NewZSpaceGameInstance);

	UFUNCTION(BlueprintCallable, meta = (NewZOrder=99))
	void ShowLoadingWidget(TSubclassOf<class UUserWidget> NewLoadingWidget, int32 NewZOrder);

	UFUNCTION(BlueprintCallable)
	void HideLoadingWidget();
	
};
