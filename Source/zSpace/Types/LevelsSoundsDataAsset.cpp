// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/LevelsSoundsDataAsset.h"
#include <Sound/SoundBase.h>

TArray<TSoftObjectPtr<USoundBase>> ULevelsSoundsDataAsset::GetSoundsByLevelName(FName LevelName)
{

	FLevelSoundsData * R_LevelSoundData =  LevelsSound.Find(LevelName);
	if (R_LevelSoundData)
	{
		return  R_LevelSoundData->Sounds;
	}
	return TArray<TSoftObjectPtr<USoundBase>>();
}
