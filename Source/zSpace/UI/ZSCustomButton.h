// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "zSpace/Types/UITypes.h"

#include "ZSCustomButton.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSCustomButton : public UButton
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedReturnThis, UZSCustomButton*, Button);

protected:
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void OnClickedEvent();
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnClickedReturnThis OnClickedReturnThis;

	UPROPERTY(EditAnywhere)
	uint8 bIsResolutionButton : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsResolutionButton"))
	EResolution Resolution;

public:
	UFUNCTION(BlueprintCallable)
	void SetResolution(EResolution NewValue);
	
};
