// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/SoundClassesDataAsset.h"
#include <Sound/SoundClass.h>

class USoundClass* USoundClassesDataAsset::GetSoundClassByName(FName SoundClassName)
{
	USoundClass* R_SoundClass = SoundClasses.Find(SoundClassName)->LoadSynchronous();
	if (IsValid(R_SoundClass))
	{
		return R_SoundClass;
	}

	return nullptr;
}
