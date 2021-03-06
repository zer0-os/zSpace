// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UChatUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* txtMessageToSend = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* ChatScrollBox = nullptr;

};
