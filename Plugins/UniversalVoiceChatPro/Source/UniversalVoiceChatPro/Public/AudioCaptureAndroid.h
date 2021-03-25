// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "MicrophoneSpeakComponent.h"

#if PLATFORM_ANDROID

#include <Android/AndroidApplication.h>
#include <Android/AndroidJNI.h>

#endif

#include "AudioCaptureAndroid.generated.h"


UCLASS()
class UNIVERSALVOICECHATPRO_API UAudioCaptureAndroid : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:
		UFUNCTION(Category = "VoiceChatUniversal")
		static bool AndroidMicrophoneStart(UMicrophoneSpeakComponent *callbackComp, int sampleRateToUse);

		UFUNCTION(Category = "VoiceChatUniversal")
			static void AndroidMicrophoneStop(UMicrophoneSpeakComponent *callbackComp);
		
		UFUNCTION(Category = "VoiceChatUniversal")
			static void AndroidAskPermission();

		UFUNCTION(Category = "VoiceChatUniversal")
			static bool AndroidHasPermission();
};