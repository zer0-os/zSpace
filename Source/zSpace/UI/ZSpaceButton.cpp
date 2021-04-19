// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/ZSpaceButton.h"

#include <Styling/SlateTypes.h>
#include "Components/Button.h"
#include <Engine/Texture2D.h>

void UZSpaceButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	auto SetImageSetting = [this](UTexture2D* Texture, FSlateBrush& SlateBrush) -> void
	{
		SlateBrush.SetResourceObject(Texture);
		SlateBrush.ImageSize = GetImageSize(Texture);
		SlateBrush.DrawAs = ESlateBrushDrawType::Type::Image;
	};

	if (IsValid(ButtonWidget))
	{
		FButtonStyle& ButtonStyle = ButtonWidget->WidgetStyle;

		SetImageSetting(ButtonNormalImage, ButtonStyle.Normal);
		SetImageSetting(ButtonHoveredImage, ButtonStyle.Hovered);
		SetImageSetting(ButtonPressedImage, ButtonStyle.Pressed);

		ButtonStyle.SetPressedSound(ButtonPresedSound);
		ButtonStyle.SetHoveredSound(ButtonHoveredSound);

		ButtonWidget->SetStyle(ButtonStyle);
	}
}

FVector2D UZSpaceButton::GetImageSize(class UTexture2D* Texture) const
{
	return IsValid(Texture) ? FVector2D(Texture->GetSizeX(), Texture->GetSizeY()) : FVector2D();
}
