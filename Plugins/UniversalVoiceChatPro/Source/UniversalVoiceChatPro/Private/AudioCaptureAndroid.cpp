// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.

#include "AudioCaptureAndroid.h"

#include "Kismet/GameplayStatics.h"
#include "MicrophoneSpeakComponent.h"

#if PLATFORM_ANDROID
#include <Android/AndroidApplication.h>
#include <Android/AndroidJNI.h>
#endif

#if PLATFORM_ANDROID
static JNIEnv* JavaEnv;
static jmethodID AndroidThunkJava_AndroidStartMicrophone = NULL;
static jmethodID AndroidThunkJava_AndroidStopMicrophone = NULL;
static jmethodID AndroidThunkJava_AndroidAskPermissionMicrophone = NULL;
static jmethodID AndroidThunkJava_AndroidHasPermissionMicrophone = NULL;
#endif



static UMicrophoneSpeakComponent *_microphoneSpeakComponentToCallback = NULL;

#if PLATFORM_ANDROID
extern "C"
{
	JNIEXPORT void Java_com_epicgames_ue4_GameActivity_dataMicPayload(JNIEnv * jni, jclass clazz, jshortArray dataMic, jint readSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("micro c++ Java_com_epicgames_ue4_GameActivity_dataMicPayload size %d"), (int) readSize);


		if (readSize > 0) {
			TArray<int16> arraydata;
			arraydata.Reserve(readSize * 2);
			jboolean isCopy;
			jshort* targetArray = jni->GetShortArrayElements(dataMic, &isCopy);

			//UE_LOG(LogTemp, Warning, TEXT("micro c++ Java_com_epicgames_ue4_GameActivity_dataMicPayload read jni array "));

			for (int32 i = 0;i < readSize;i++) {
				jshort sht = targetArray[i];
				arraydata.Add(sht);
				//uint8 c1 = sht & 0xFF;
				//uint8 c2 = sht >> 8;
				//arraydata.Add(c1);			
				//arraydata.Add(c2); 			
			}
			jni->ReleaseShortArrayElements(dataMic, targetArray, 0);

			if (_microphoneSpeakComponentToCallback != NULL && _microphoneSpeakComponentToCallback->IsValidLowLevel() && !_microphoneSpeakComponentToCallback->IsPendingKill()) {
				_microphoneSpeakComponentToCallback->receiveDataMicrophone(arraydata, readSize);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("micro c++ Java_com_epicgames_ue4_GameActivity_dataMicPayload _microphoneSpeakComponentToCallback no valid"));
			}
			
		}
	} 

	
}
#endif

bool UAudioCaptureAndroid::AndroidMicrophoneStart(UMicrophoneSpeakComponent *callbackComp,int sampleRateToUse)
{
	_microphoneSpeakComponentToCallback = callbackComp;


#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("micro c++ AndroidThunkJava_AndroidStartMicrophone %d"), sampleRateToUse);

	JavaEnv = FAndroidApplication::GetJavaEnv();

	if (JavaEnv && !AndroidThunkJava_AndroidStartMicrophone)
	{
		AndroidThunkJava_AndroidStartMicrophone = FJavaWrapper::FindMethod(JavaEnv, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AndroidStartMicrophone", "(I)Z", false);
	}

	if (!AndroidThunkJava_AndroidStartMicrophone || !JavaEnv) {
		UE_LOG(LogTemp, Warning, TEXT("micro c++ Error val AndroidThunkJava_AndroidStartMicrophone %d %d "), AndroidThunkJava_AndroidStartMicrophone == NULL, JavaEnv == NULL);
		return false;
	}

	jboolean res = FJavaWrapper::CallBooleanMethod(JavaEnv, FJavaWrapper::GameActivityThis, AndroidThunkJava_AndroidStartMicrophone, sampleRateToUse);

	return res;
#else
	return false;
#endif
}

void UAudioCaptureAndroid::AndroidMicrophoneStop(UMicrophoneSpeakComponent *callbackComp) {

	_microphoneSpeakComponentToCallback = NULL;

#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("micro c++ AndroidThunkJava_AndroidStopMicrophone "));

	JavaEnv = FAndroidApplication::GetJavaEnv();

	if (JavaEnv && !AndroidThunkJava_AndroidStopMicrophone)
	{
		AndroidThunkJava_AndroidStopMicrophone = FJavaWrapper::FindMethod(JavaEnv, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AndroidStopMicrophone", "()V", false);
	}

	if (!AndroidThunkJava_AndroidStopMicrophone || !JavaEnv) {
		UE_LOG(LogTemp, Warning, TEXT("micro c++ Error val AndroidThunkJava_AndroidStopMicrophone %d %d "), AndroidThunkJava_AndroidStopMicrophone == NULL, JavaEnv == NULL);
		return;
	}

	FJavaWrapper::CallVoidMethod(JavaEnv, FJavaWrapper::GameActivityThis, AndroidThunkJava_AndroidStopMicrophone);

#endif
}


void UAudioCaptureAndroid::AndroidAskPermission()
{
#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("micro c++ AndroidThunkJava_AndroidAskPermissionMicrophone "));

	JavaEnv = FAndroidApplication::GetJavaEnv();

	if (JavaEnv && !AndroidThunkJava_AndroidAskPermissionMicrophone)
	{
		AndroidThunkJava_AndroidAskPermissionMicrophone = FJavaWrapper::FindMethod(JavaEnv, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AndroidAskPermissionMicrophone", "()V", false);
	}

	if (!AndroidThunkJava_AndroidAskPermissionMicrophone || !JavaEnv) {
		UE_LOG(LogTemp, Warning, TEXT("micro c++ Error val AndroidThunkJava_AndroidAskPermissionMicrophone %d %d "), AndroidThunkJava_AndroidAskPermissionMicrophone == NULL, JavaEnv == NULL);
		return;
	}

	FJavaWrapper::CallVoidMethod(JavaEnv, FJavaWrapper::GameActivityThis, AndroidThunkJava_AndroidAskPermissionMicrophone);

#endif
}

bool UAudioCaptureAndroid::AndroidHasPermission()
{
#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("micro c++ AndroidThunkJava_AndroidHasPermissionMicrophone "));

	JavaEnv = FAndroidApplication::GetJavaEnv();

	if (JavaEnv && !AndroidThunkJava_AndroidAskPermissionMicrophone)
	{
		AndroidThunkJava_AndroidHasPermissionMicrophone = FJavaWrapper::FindMethod(JavaEnv, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AndroidHasPermissionMicrophone", "()Z", false);
	}

	if (!AndroidThunkJava_AndroidHasPermissionMicrophone || !JavaEnv) {
		UE_LOG(LogTemp, Warning, TEXT("micro c++ Error val AndroidThunkJava_AndroidHasPermissionMicrophone %d %d "), AndroidThunkJava_AndroidHasPermissionMicrophone == NULL, JavaEnv == NULL);
		return false;
	}

	jboolean hasPermision = FJavaWrapper::CallBooleanMethod(JavaEnv, FJavaWrapper::GameActivityThis, AndroidThunkJava_AndroidHasPermissionMicrophone);
	bool res = hasPermision;
	UE_LOG(LogTemp, Warning, TEXT("micro c++ Error val AndroidThunkJava_AndroidHasPermissionMicrophone hasPermission %d res %d"), hasPermision, res);
	return res;

#else
	return false;
#endif
	
}