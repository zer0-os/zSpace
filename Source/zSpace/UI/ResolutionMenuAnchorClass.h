// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"

#include "ResolutionMenuAnchorClass.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UResolutionMenuAnchorClass : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TMap<EResolution, class UButton*> ResolutionAndButton;
	
public:
	UFUNCTION(BlueprintCallable)
	UButton* CreateButton(const FButtonStyle ButtonStyle, EResolution Resolution);
	
};
