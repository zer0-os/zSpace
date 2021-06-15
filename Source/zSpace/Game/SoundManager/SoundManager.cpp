// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/SoundManager/SoundManager.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>
#include "zSpace/Types/SoundClassesDataAsset.h"
#include <Sound/SoundMix.h>
#include <Sound/SoundClass.h>


void USoundManager::PlayPortalSoundByType(EPortalSoundType SoundType)
{
	if (IsValid(PortalSoundsDataAsset))
	{
		USoundBase* L_Sound = nullptr;
		if (SoundType == EPortalSoundType::FadeIn)
		{
			L_Sound = PortalSoundsDataAsset->PortalFadeInSound;
		}
		if (SoundType == EPortalSoundType::FadeOut)
		{
			L_Sound = PortalSoundsDataAsset->PortalFadeOutSound;
		}
		if (SoundType == EPortalSoundType::MidLoop)
		{
			L_Sound = PortalSoundsDataAsset->PortalMiddleSound;
		}

		if (IsValid(L_Sound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), L_Sound);
		}
	}

}

void USoundManager::SetSoundVolumeBySoundClass(FName SoundClassName, float Volume)
{
	class USoundMix * L_GlobalSoundMix = SoundClassesDataAsset->GlobalSoundMix;
	class USoundClass * L_SoundClasses = SoundClassesDataAsset->GetSoundClassByName(SoundClassName);

	if(IsValid(L_GlobalSoundMix) && IsValid(L_SoundClasses))
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), L_GlobalSoundMix, L_SoundClasses, Volume, 1.f, 0);
	}
}

