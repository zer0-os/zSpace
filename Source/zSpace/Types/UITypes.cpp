// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/UITypes.h"

#include "zSpace/UI/SelectCharacterUserWidget.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/NoExportTypes.h"

FIntPoint UConvertEResolutionToFIntPointOrViceVersa::GetIntPoint(const UObject* WorldContext, EResolution Resolution)
{
	switch (Resolution)
	{
	case EResolution::R_5120X1440:
		return FIntPoint(5120, 1440);
		break;
	case EResolution::R_1920X1080:
		return FIntPoint(1920, 1080);
		break;
	case EResolution::R_1440X900:
		return FIntPoint(1440, 900);
		break;
	default:
		return FIntPoint::ZeroValue;
		break;
	}
}

EResolution UConvertEResolutionToFIntPointOrViceVersa::GetEnumResolution(const UObject* WorldContext, FIntPoint Resolution)
{
	float ErrorTolerance = 0.f;
	UWorld *World = WorldContext->GetWorld();
	if (IsValid(WorldContext))
	{
		const EWorldType::Type WorldType = World->WorldType;
		if (WorldType == EWorldType::PIE)
		{
			ErrorTolerance = 100.f;
		}
	}
	
	auto Compare = [ErrorTolerance](const float A, const float B) -> bool
	{
		return UKismetMathLibrary::NearlyEqual_FloatFloat(A, B, ErrorTolerance);
	};
	
	if (Compare(Resolution.X, 5120.f) && Compare(Resolution.Y, 1440.f)) return EResolution::R_5120X1440;
	if (Compare(Resolution.X, 1920.f) && Compare(Resolution.Y, 1080.f)) return EResolution::R_1920X1080;
	if (Compare(Resolution.X, 1440.f) && Compare(Resolution.Y, 900.f)) return EResolution::R_1440X900;

	return EResolution::None;
}

FButtonStyle UButtonStyleByResolution::GetButtonStyleByResolution(const UObject* WorldContext,
	EResolution Resolution, const EResolutionButtonStyleType ResolutionButtonStyle, UButtonStyleByResolution* ButtonStyleByResolution)
{
	auto Data = ResolutionButtonStyle == EResolutionButtonStyleType::ActiveStyle ?
		ButtonStyleByResolution->ActiveStyle : ButtonStyleByResolution->NormalStyle;

 	FButtonStyle* Style = Data.Find(Resolution);
	if (Style)
	{
		return *Style;
	}

	return FButtonStyle();
}

TSubclassOf<class UUserWidget> UResolutionAndWidgetDataAsset::GetWidget(EResolution ResolutionEnum)
{	
	const TSubclassOf<class UUserWidget>* Find = ResolutionAndWidget.ResolutionAndWidget.Find(ResolutionEnum);
	if (Find)
	{
		return *Find;
	}

	return nullptr;
}
