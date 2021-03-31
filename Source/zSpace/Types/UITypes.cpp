// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/UITypes.h"

FIntPoint UConverEResolutionToFIntPointOrViceVersa::GetIntPoint(const UObject* WorldContext, EResolution Resolution)
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

EResolution UConverEResolutionToFIntPointOrViceVersa::GetEnumResolution(const UObject* WorldContext, FIntPoint Resolution)
{
	if (Resolution.X == 5120 && Resolution.Y == 1440) return EResolution::R_5120X1440;
	if (Resolution.X == 1920 && Resolution.Y == 1080) return EResolution::R_1920X1080;
	if (Resolution.X == 1440 && Resolution.Y == 900) return EResolution::R_1440X900;

	return EResolution::None;
}

TSubclassOf<class UUserWidget> UResolutionAndWidget::GetWidget(EResolution ResolutionEnum)
{	
	const TSubclassOf<class UUserWidget>* Find = ResolutionAndWidget.ResolutionAndWidget.Find(ResolutionEnum);
	if (Find)
	{
		return *Find;
	}

	return nullptr;
}
