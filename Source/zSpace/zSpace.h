// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Engine.h"
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogZSpace, Log, All);

DEFINE_LOG_CATEGORY(LogZSpace);

#define PRINT(X) UKismetSystemLibrary::PrintString(this, X)
#define PRINT_FLOAT(X) UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(X))
#define PRINT_TIME(X, T) UKismetSystemLibrary::PrintString(this, X, true, false, FLinearColor::Blue, T)

template<typename T>
FString EnumToString(const TCHAR* ObjEnameName, T Value)
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ObjEnameName, true);
	if (IsValid(EnumPtr))
	{
		const FString ReplaceStr = FString::Printf(TEXT("%s::"), ObjEnameName);
		const FName& EnumName = EnumPtr->GetNameByIndex(static_cast<uint8>(Value));
		const FString EnumStr = EnumName.ToString().Replace(*ReplaceStr, TEXT(""));
		return EnumStr;
	}

	return FString();
}
