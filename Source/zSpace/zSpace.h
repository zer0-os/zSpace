// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Engine.h"
#include "CoreMinimal.h"

#define PRINT(X) UKismetSystemLibrary::PrintString(this, X)
#define PRINT_FLOAT(X) UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(X))
#define PRINT_TIME(X, T) UKismetSystemLibrary::PrintString(this, X, true, false, FLinearColor::Blue, T)

