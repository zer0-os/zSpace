// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/CharacterUnderFootSurfaceDA/CharacterUnderFootSurfaceDA.h"

FFootHitGroundEqualData::FFootHitGroundEqualData()
{
	MovementMode = EMovementMode::MOVE_None;
	PreviousMovementMode = EMovementMode::MOVE_None;
}

FFootHitGroundData::FFootHitGroundData()
{
	MovementMode = EMovementMode::MOVE_None;
	PreviousMovementMode = EMovementMode::MOVE_None;
	SoundBase = nullptr;
}

bool FFootHitGroundData::operator==(const FFootHitGroundData& NewFootHitGroundData)
{
	return NewFootHitGroundData.PreviousMovementMode == PreviousMovementMode && NewFootHitGroundData.SoundBase == SoundBase;
}

bool operator==(FFootHitGroundEqualData & NewFootHitGroundEqualDataA, const FFootHitGroundData& NewFootHitGroundDataB)
{
	return NewFootHitGroundEqualDataA.PreviousMovementMode == NewFootHitGroundDataB.PreviousMovementMode
	    && NewFootHitGroundEqualDataA.MovementMode == NewFootHitGroundDataB.MovementMode;
}

bool operator==(const FFootHitGroundData& NewFootHitGroundDataA, FFootHitGroundEqualData & NewFootHitGroundEqualDataB)
{
	return NewFootHitGroundDataA.PreviousMovementMode == NewFootHitGroundEqualDataB.PreviousMovementMode
	    && NewFootHitGroundDataA.MovementMode == NewFootHitGroundEqualDataB.MovementMode;
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

FFootHitGroundData UCharacterUnderFootSurfaceDA::GetFootHitGroundDataByMovementMode( EMovementMode NewPreviousMovementMode, EMovementMode NewCurrentMovementMode, bool& NewIsValid)
{
	NewIsValid = false;
	FFootHitGroundData R_FootHitGroundData;
	for(const FFootHitGroundData & Iter : FootHitGroundDataArray)
	{
		FFootHitGroundEqualData L_FootHitGroundEqualData;
		L_FootHitGroundEqualData.PreviousMovementMode = NewPreviousMovementMode;
		L_FootHitGroundEqualData.MovementMode = NewCurrentMovementMode;
		if(Iter == L_FootHitGroundEqualData)
		{
			NewIsValid = true;
			R_FootHitGroundData = Iter;
		}
	}
	return R_FootHitGroundData;
}
