// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/CharacterUnderFootSurfaceDA/CharacterUnderFootSurfaceDA.h"

TArray<FCharacterUnderFootSurfaceData> UCharacterUnderFootSurfaceDA::GetCharacterUnderFootSurfaceDataArray() const
{
	return CharacterUnderFootSurfaceDataArray;
}

FCharacterUnderFootSurfaceData UCharacterUnderFootSurfaceDA::GetCharacterUnderFootSurfaceDataByPhysicsType(
	const UPhysicalMaterial * NewPhysicalMaterial, bool & NewIsValid)
{
	NewIsValid = false;
	FCharacterUnderFootSurfaceData R_CharacterUnderFootSurfaceData;
	if(NewPhysicalMaterial)
	{
		for(const auto & Iter : CharacterUnderFootSurfaceDataArray)
		{
			if(Iter.PhysicalMaterial == NewPhysicalMaterial)
			{
				R_CharacterUnderFootSurfaceData = Iter;
				NewIsValid = true;
				break;
			}
		}
	}
	return R_CharacterUnderFootSurfaceData;	
}
