// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.
#pragma once

#if PLATFORM_IOS
#include "AudioCaptureCore.h"
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AudioUnit.h>
#include "AVFoundation/AVFoundation.h"
#include "Math/UnrealMathUtility.h"



class UMicrophoneSpeakComponent;

	class FAudioCaptureIOS 
	{
	public:
		FAudioCaptureIOS();

		bool GetCaptureDeviceInfo(Audio::FCaptureDeviceInfo& OutInfo, int32 DeviceIndex);
		bool iosOpenCaptureStream(int _SampleRate, int _NumChannels, int32 encoderMinimumBytesPerFrame );
		 bool CloseStream() ;
		 bool StartStream() ;
		 bool StopStream() ;
		 bool AbortStream() ;
		 bool GetStreamTime(double& OutStreamTime) ;
		 int32 GetSampleRate() const  { return SampleRate; }
		
		 bool IsStreamOpen() const ;
		 bool IsCapturing() const ;
		 void OnAudioCapture(void* InBuffer, uint32 InBufferFrames, double StreamTime, bool bOverflow) ;
		 bool GetInputDevicesAvailable(TArray<Audio::FCaptureDeviceInfo>& OutDevices) ;
		 void SetHardwareFeatureEnabled(Audio::EHardwareInputFeature FeatureType, bool bEnabled);

		OSStatus OnCaptureCallback(AudioUnitRenderActionFlags* ioActionFlags, const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList* ioData);

		UMicrophoneSpeakComponent *callbackSpeakMicrophone;

	private:
		void AllocateBuffer(int SizeInBytes);

		bool streamOpened = false;
		bool wasInit = false;
		AudioComponentInstance IOUnit;

		int32 NumChannels;
		int32 SampleRate;
		int32 encoderMinimumBytesPerFrame;

		TArray<uint8> CaptureBuffer;
		TArray<int16> VoiceCaptureBufferAppended;
		int BufferSize = 0;
	};

#endif