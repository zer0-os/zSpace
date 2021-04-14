// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZSpaceButton.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSpaceButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	FVector2D GetImageSize(class UTexture2D* Texture) const;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* ButtonWidget = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* ButtonBackgroundImage = nullptr;

	UPROPERTY(EditAnywhere, Category="Button")
	class UTexture2D* ButtonNormalImage = nullptr;

	UPROPERTY(EditAnywhere, Category="Button")
	class UTexture2D* ButtonHoveredImage = nullptr;

	UPROPERTY(EditAnywhere, Category="Button")
	class UTexture2D* ButtonPressedImage = nullptr;
};
