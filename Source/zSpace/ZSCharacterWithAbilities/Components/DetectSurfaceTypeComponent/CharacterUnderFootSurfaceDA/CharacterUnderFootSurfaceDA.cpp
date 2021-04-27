// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/CharacterUnderFootSurfaceDA/CharacterUnderFootSurfaceDA.h"

FFootHitGroundData::FFootHitGroundData()
{
	MovementMode = EMovementMode::MOVE_None;
	PreviousMovementMode = EMovementMode::MOVE_None;
	SoundBase = nullptr;
}

bool FFootHitGroundData::operator==(const FFootHitGroundData& NewFootHitGroundData)
{
	return NewFootHitGroundData.MovementMode == MovementMode && NewFootHitGroundData.SoundBase == SoundBase;
}

bool operator==(EMovementMode& NewMovementMode, const FFootHitGroundData& NewFootHitGroundDataB)
{
	return NewMovementMode == NewFootHitGroundDataB.MovementMode;
}

bool operator==(const FFootHitGroundData& NewFootHitGroundDataA, EMovementMode& NewMovementModeB)
{
	return NewFootHitGroundDataA.MovementMode == NewMovementModeB;
}


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

FFootHitGroundData UCharacterUnderFootSurfaceDA::GetFootHitGroundDataByMovementMode( EMovementMode MovementMode, bool& NewIsValid)
{
	NewIsValid = false;
	FFootHitGroundData R_FootHitGroundData;
	for(const FFootHitGroundData & Iter : FootHitGroundDataArray)
	{
		if(Iter == MovementMode)
		{
			NewIsValid = true;
			R_FootHitGroundData = Iter;
		}
	}
	return R_FootHitGroundData;
}
