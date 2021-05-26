// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/SoundManager/SoundManager.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

void USoundManager::PlayPortalSoundByType(EPortalSoundType SoundType)
{
	if (IsValid(PortalSounds))
	{
		USoundBase* L_Sound = nullptr;
		if (SoundType == EPortalSoundType::FadeIn)
		{
			L_Sound = PortalSounds->PortalFadeInSound;
		}
		if (SoundType == EPortalSoundType::FadeOut)
		{
			L_Sound = PortalSounds->PortalFadeOutSound;
		}
		if (SoundType == EPortalSoundType::MidLoop)
		{
			L_Sound = PortalSounds->PortalMiddleSound;
		}

		if (IsValid(L_Sound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), L_Sound);
		}
	}
}

