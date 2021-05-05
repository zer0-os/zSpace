// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZSAnimationTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerGait : uint8
{
	Standing = 0,
	Walking = 1,
	Running = 2,
	Sprinting = 3,
};
