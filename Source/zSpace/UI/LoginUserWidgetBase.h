// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSLoginWidget.h"
#include "LoginUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API ULoginUserWidgetBase : public UOWSLoginWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidgetOptional))
	class UEditableTextBox* txtEmail = nullptr;


};
