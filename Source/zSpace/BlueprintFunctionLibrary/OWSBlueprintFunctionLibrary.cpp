// Copyright 2020 Sabre Dart Studios


#include "zSpace/BlueprintFunctionLibrary/OWSBlueprintFunctionLibrary.h"

FString UOWSBlueprintFunctionLibrary::GetCharacterNameFromMeshFieldName(const UObject* WorldContext,
	const FString& MeshFieldName)
{
	// Field Name
	FString* LeftS = nullptr;
	// Character Name
	FString* RightS = nullptr;
	
	MeshFieldName.Split("_", LeftS, RightS);
	
	return RightS ? *RightS : FString();
}
