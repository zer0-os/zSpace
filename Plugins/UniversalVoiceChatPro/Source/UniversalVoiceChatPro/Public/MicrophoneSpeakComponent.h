// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"

#include "VoiceModule.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#if PLATFORM_IOS
#include "AVFoundation/AVFoundation.h"
#include "AudioCaptureIOS.h"
#endif


#include "MicrophoneSpeakComponent.generated.h"


#define NUM_ENTROPY_VALUES 5

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSALVOICECHATPRO_API UMicrophoneSpeakComponent : public UActorComponent
{
	GENERATED_BODY()

		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDataMicrophoneReceived, const TArray<uint8>&, data);

public:
	UMicrophoneSpeakComponent();

	UPROPERTY(BlueprintAssignable, Category = "VoiceChatUniversal")
		FDataMicrophoneReceived OnDataMicrophoneReceived;
		

	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		bool isPlayingAudioVoice();

	/* create sound wave, audio component */
	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
	bool initAudioResources(int32 voiceSampleRate, int32 _voiceNumChannels);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		bool startSpeaking(bool _shouldHearMyOwnVoice, bool isGlobal, int radioChannel, bool useRange, float maxRange);
	
	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		void endSpeaking();
		
	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		void muteAudio(bool _isMuted);


	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		void SetVoiceVolume(float volume);

	// client transmit voice
	UFUNCTION(Server, Unreliable, Category = "VoiceChatUniversal")
		void RPCClientTransmitVoiceData(TArray<uint8> const &data, int32 sampleRate, int32 numchannels, int32 PCMSize, bool _isGlobal, int _radioChannel, bool _useRange, float _maxRange);

	UFUNCTION(NetMulticast , Unreliable, Category = "VoiceChatUniversal")
		void RPCServerBroadcastVoiceData(TArray<uint8> const &data, int32 sampleRate, int32 numchannels, int32 PCMSize);
	// server send voice to relevant client
	UFUNCTION(Client, Unreliable, Category = "VoiceChatUniversal")
	void RPCReceiveVoiceFromServer(UMicrophoneSpeakComponent *compToOutputVoice, TArray<uint8> const &dataEncoded, int32 sampleRate, int32 numchannels, int32 PCMSize);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatUniversal")
		void payloadReceivedVoiceData(TArray<uint8> const &dataEncoded, int32 sampleRate, int32 numchannels, int32 PCMSize);

	UFUNCTION(Category = "VoiceChatUniversal")
		void setAttenuationAssetPath(bool enableAttenuation, FString _pathToAttenuationAsset);


	// variable used for local client voice
	bool isSpeakingLocalSetting = false;
	bool shouldHearMyOwnVoiceLocalSetting = true;
	bool isMutedLocalSetting = false;
	bool isGlobalLocalSetting = true;
	int radioChannelLocalSetting = 0;
	bool useRangeLocalSettings = false;
	float maxRangeLocalSettings = 0;

	FString pathToAttenuationAsset ="";
	bool attenuationIsEnabled = false;

	UPROPERTY()
	UAudioComponent* VoiceCaptureAudioComponent;
	UPROPERTY()
	USoundWaveProcedural* VoiceCaptureSoundWaveProcedural;
	
	/* mobile raw data buffer for opus */
	TArray<uint8> MobileVoiceCaptureBufferAppended;

	/* opus buffer used */
	TArray<uint8> bufferDecodedOpus;
	TArray<uint8> bufferEncodedOpus;	
	TArray<uint8> bufferRPCdecodedData;

	

#if PLATFORM_WINDOWS
	// PC
	TSharedPtr<class IVoiceCapture> PCVoiceCapture;
	TArray<uint8> PCVoiceCaptureBuffer;
	TArray<uint8> PCVoiceCaptureBufferAppended;
	int32 MaxRawCaptureDataSize;
#endif


#if PLATFORM_IOS
	FAudioCaptureIOS iosCapture;
#endif



	//USynthComponent *synthComp;
	//USynthSound *synthSound;

	//UAudioCaptureExtended *audextend;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void OnUnregister() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void receiveDataMicrophone(TArray<int16> data, int32 readsize);
	void transmitDataToOtherAndLocal(uint8 *pointerBufferPCMMic, int32 uint8sizeBufferPCMMic);
	bool getWasInitAudioResources();

private:
	
	/* opus init, encode, decode*/
	bool OpusInit(int32 InSampleRate, int32 InNumChannels);
	int32 OpusEncode(const uint8* RawPCMData, uint32 RawDataSize, uint8* OutCompressedData, uint32& OutCompressedDataSize);
	bool OpusDecoderInit(int32 InSampleRate, int32 InNumChannels);
	void OpusDecode(const uint8* InCompressedData, uint32 CompressedDataSize, uint8* OutRawPCMData, uint32& OutRawDataSize);
	/* start playing sound */
	void PlayAudioVoice();

	/* was already init ?*/
	bool wasInitAudioResources = false;

	/*********************** OPUS ************************/
	/** Sample rate encoding (supports 8000, 12000, 16000, 24000, 480000) */
	int32 voiceSampleRate;
	/** Encoded channel count (supports 1,2) */
	int32 voiceNumChannels;

	/** Sample rate encoding (supports 8000, 12000, 16000, 24000, 480000) */
	int32 encoderSampleRate;
	/** Encoded channel count (supports 1,2) */
	int32 encoderNumChannels;
	/**
	 * Number of samples encoded in a time slice "frame" (must match decoder)
	 * One frame defined as (2.5, 5, 10, 20, 40 or 60 ms) of audio data
	 * Voice encoding lower bound is 10ms (audio goes to 2.5ms).
	 * Voice encoding upper bound is 60ms (audio goes to 20ms).
	 * at 48 kHz the permitted values are 120 (2.5ms), 240 (5ms), 480 (10ms), 960 (20ms), 1920 (40ms), and 2880 (60ms)
	 */
	int32 encoderFrameSize;

	int32 encoderBytesPerFrame;

	/** Opus encoder stateful data */
	struct OpusEncoder* Encoder;
	/** Last values for error checking with the decoder */
	uint32 encoderEntropy[NUM_ENTROPY_VALUES];
	/** Last recorded entropy index */
	uint32 encoderLastEntropyIdx;
	/** Last value set in the call to Encode() */
	uint8 encoderGeneration;


	/** Sample rate to decode into, regardless of encoding (supports 8000, 12000, 16000, 24000, 480000) */
	int32 decoderSampleRate;
	/** Decoded channel count (supports 1,2) */
	int32 decoderNumChannels;
	/**
	 * Number of samples encoded in a time slice (must match encoder)
	 * at 48 kHz the permitted values are 120, 240, 480, 960, 1920, and 2880
	 */
	int32 decoderFrameSize;
	/** Opus decoder stateful data */
	struct OpusDecoder* Decoder;
	/** Last values for error checking with the encoder */
	uint32 decoderEntropy[NUM_ENTROPY_VALUES];
	/** Last recorded entropy index */
	uint32 decoderLastEntropyIdx;
	/** Generation value received from the last incoming packet */
	uint8 decoderLastGeneration;
	/*******************************************************/
};


