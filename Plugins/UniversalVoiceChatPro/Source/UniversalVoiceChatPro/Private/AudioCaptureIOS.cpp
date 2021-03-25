// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.

#if PLATFORM_IOS
#include "AudioCaptureIOS.h"

const int32 kInputBus = 1;
const int32 kOutputBus = 0;

FAudioCaptureIOS::FAudioCaptureIOS()
	: NumChannels(0)
	, SampleRate(0)
{
}

static OSStatus RecordingCallback(void *inRefCon,
	AudioUnitRenderActionFlags *ioActionFlags,
	const AudioTimeStamp *inTimeStamp,
	UInt32 inBusNumber,
	UInt32 inNumberFrames,
	AudioBufferList *ioData)
{
	FAudioCaptureIOS* AudioCapture = (FAudioCaptureIOS*)inRefCon;
	
	
	return AudioCapture->OnCaptureCallback(ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);
}

OSStatus FAudioCaptureIOS::OnCaptureCallback(AudioUnitRenderActionFlags* ioActionFlags, const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList* ioData)
{
	OSStatus status = noErr;

	const int NeededBufferSize = inNumberFrames * NumChannels * sizeof(float);
	if (CaptureBuffer.Num() == 0 || BufferSize < NeededBufferSize)
	{
		BufferSize = NeededBufferSize;
		AllocateBuffer(BufferSize);
	}

	AudioBufferList* BufferList = (AudioBufferList*)CaptureBuffer.GetData();
	for (int i = 0; i < BufferList->mNumberBuffers; ++i) {
		BufferList->mBuffers[i].mDataByteSize = (UInt32)BufferSize;
	}

	status = AudioUnitRender(IOUnit,
		ioActionFlags,
		inTimeStamp,
		inBusNumber,
		inNumberFrames,
		BufferList);
	if (status != noErr) {
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitRender error %d"), status);
		return 0;
	}

	
	void* InBuffer = (void*)BufferList->mBuffers[0].mData; // only first channel ?!
	OnAudioCapture(InBuffer, inNumberFrames, 0.0, false); // need calculate timestamp

	

	return noErr;
}

void FAudioCaptureIOS::AllocateBuffer(int SizeInBytes)
{
	size_t NeedBytes = sizeof(AudioBufferList) + NumChannels * (sizeof(AudioBuffer) + SizeInBytes);

	CaptureBuffer.SetNum(NeedBytes);

	AudioBufferList* list = (AudioBufferList*)CaptureBuffer.GetData();
	uint8* data = CaptureBuffer.GetData() + sizeof(AudioBufferList) + sizeof(AudioBuffer);

	list->mNumberBuffers = NumChannels;
	for (int i = 0; i < NumChannels; i++)
	{
		list->mBuffers[i].mNumberChannels = 1;
		list->mBuffers[i].mDataByteSize = (UInt32)SizeInBytes;
		list->mBuffers[i].mData = data;

		data += (SizeInBytes + sizeof(AudioBuffer));
	}
}

bool FAudioCaptureIOS::GetCaptureDeviceInfo(Audio::FCaptureDeviceInfo& OutInfo, int32 DeviceIndex)
{
	OutInfo.DeviceName = FString(TEXT("Remote IO Audio Component"));
	OutInfo.InputChannels = 1;
	OutInfo.PreferredSampleRate = 48000;

	return true;
}


bool FAudioCaptureIOS::iosOpenCaptureStream(int _SampleRate, int _NumChannels, int32 _encoderMinimumBytesPerFrame)
{
	if (wasInit) {
		UE_LOG(LogTemp, Warning, TEXT("iosOpenCaptureStream was already init"));
		return true;
	}

	
	UE_LOG(LogTemp, Warning, TEXT("iosOpenCaptureStream %d %d"), _SampleRate, _NumChannels);
	NumChannels = _NumChannels;
	SampleRate = _SampleRate;
	encoderMinimumBytesPerFrame = _encoderMinimumBytesPerFrame;
	OSStatus Status = noErr;

	/* config audiosession */
	NSError *setCategoryError = nil;
	[[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord withOptions : (AVAudioSessionCategoryOptionDefaultToSpeaker | AVAudioSessionCategoryOptionAllowBluetooth) error : &setCategoryError];
	if (setCategoryError != nil)
	{		
		UE_LOG(LogTemp, Warning, TEXT("error AVAudioSessionCategoryPlayAndRecord"));
		return false;
	}

	[[AVAudioSession sharedInstance] setActive:YES error : &setCategoryError];
	if (setCategoryError != nil)
	{
		UE_LOG(LogTemp, Warning, TEXT("error setActive"));
		return false;
	}

	// Source of info "Technical Note TN2091 - Device input using the HAL Output Audio Unit"

	AudioComponentDescription desc;
	desc.componentType = kAudioUnitType_Output;
	// We use processing element always for enable runtime changing HW AEC and AGC settings.
	// When it's disable the unit work as RemoteIO
	desc.componentSubType = kAudioUnitSubType_VoiceProcessingIO;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;

	AudioComponent InputComponent = AudioComponentFindNext(NULL, &desc);

	Status = AudioComponentInstanceNew(InputComponent, &IOUnit);
	if (Status != noErr) {
		UE_LOG(LogTemp, Warning, TEXT("AudioComponentInstanceNew error %d"), Status);		
		return false;
	}
	// Enable recording
	uint32 EnableIO = 1;
	Status = AudioUnitSetProperty(IOUnit,
		kAudioOutputUnitProperty_EnableIO,
		kAudioUnitScope_Input,
		kInputBus,
		&EnableIO,
		sizeof(EnableIO));
	if (Status != noErr) {
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitSetProperty error %d"), Status);
		return false;
	}

	// Disable output part
	EnableIO = 0;
	Status = AudioUnitSetProperty(IOUnit,
		kAudioOutputUnitProperty_EnableIO,
		kAudioUnitScope_Output,
		kOutputBus,
		&EnableIO,
		sizeof(EnableIO));
	if (Status != noErr) {
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitSetProperty 1 error %d"), Status);		
		return false;
	}

	AudioStreamBasicDescription StreamDescription = { 0 };
	const UInt32 BytesPerSample = sizeof(Float32);

	StreamDescription.mSampleRate = SampleRate;
	StreamDescription.mFormatID = kAudioFormatLinearPCM;
	StreamDescription.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
	StreamDescription.mChannelsPerFrame = NumChannels;
	StreamDescription.mBitsPerChannel = 8 * BytesPerSample;
	StreamDescription.mBytesPerFrame = BytesPerSample * StreamDescription.mChannelsPerFrame;
	StreamDescription.mFramesPerPacket = 1;
	StreamDescription.mBytesPerPacket = StreamDescription.mFramesPerPacket * StreamDescription.mBytesPerFrame;

	// Configure output format
	Status = AudioUnitSetProperty(IOUnit,
		kAudioUnitProperty_StreamFormat,
		kAudioUnitScope_Output,
		kInputBus,
		&StreamDescription,
		sizeof(StreamDescription));
	if (Status != noErr) {		
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitSetProperty 2error %d"), Status);
		return false;
	}

	// Setup capture callback
	AURenderCallbackStruct CallbackInfo;
	CallbackInfo.inputProc = RecordingCallback;
	CallbackInfo.inputProcRefCon = this;
	Status = AudioUnitSetProperty(IOUnit,
		kAudioOutputUnitProperty_SetInputCallback,
		kAudioUnitScope_Global,
		kInputBus,
		&CallbackInfo,
		sizeof(CallbackInfo));
	if (Status != noErr) {		
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitSetProperty 3 error %d"), Status);
		return false;
	}

	// Initialize audio unit
	Status = AudioUnitInitialize(IOUnit);
	if (Status != noErr) {		
		UE_LOG(LogTemp, Warning, TEXT("AudioUnitInitialize error %d"), Status);
		return false;
	}

	wasInit = true;
	return Status == noErr;
}

bool FAudioCaptureIOS::CloseStream()
{
	StopStream();
	AudioComponentInstanceDispose(IOUnit);
	return true;
}

bool FAudioCaptureIOS::StartStream()
{
	if (streamOpened) return true;
	
	AudioUnitReset(IOUnit, kAudioUnitScope_Global, 0);
	/* config audiosession */
	/*NSError *setCategoryError = nil;
	[[AVAudioSession sharedInstance] setActive:YES error : &setCategoryError];
	if (setCategoryError != nil)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("error setActive ")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("StartStream setActive ")));
	*/
	bool res = (AudioOutputUnitStart(IOUnit) == noErr);
	streamOpened = res;
	return res;
}

bool FAudioCaptureIOS::StopStream()
{
	if (!streamOpened) return false;
	bool res = (AudioOutputUnitStop(IOUnit) == noErr);
	streamOpened = !res;
	return res;
}

bool FAudioCaptureIOS::AbortStream()
{
	StopStream();
	CloseStream();
	return true;
}

bool FAudioCaptureIOS::GetStreamTime(double& OutStreamTime)
{
	OutStreamTime = 0.0f;
	return true;
}

bool FAudioCaptureIOS::IsStreamOpen() const
{
	return streamOpened;
}

bool FAudioCaptureIOS::IsCapturing() const
{
	return streamOpened;
}

void FAudioCaptureIOS::OnAudioCapture(void* InBuffer, uint32 InBufferFrames, double StreamTime, bool bOverflow)
{
	float* InBufferData = (float*)InBuffer;
	
	TArray<int16> buffpass;
	buffpass.Reserve(InBufferFrames * 10);
	for (uint32 i = 0; i < InBufferFrames; i++) {
		buffpass.Add((int16)FMath::Clamp(InBufferData[i] * 32768.0f, -32768.0f, 32767.0f));
	}
	//buffpass.SetNum(InBufferFrames);
	//memcpy(buffpass.GetData(), InBuffer, sizeof(float) * InBufferFrames);
	// callback to microphone component

	VoiceCaptureBufferAppended.Append(buffpass);


	
	int sizeBufferInUint = VoiceCaptureBufferAppended.Num() * 2;
	int amountWillBeRemind = sizeBufferInUint % encoderMinimumBytesPerFrame;
	int frameSentToOpus = sizeBufferInUint / encoderMinimumBytesPerFrame;
	int startOffsetRemind = (sizeBufferInUint  / encoderMinimumBytesPerFrame) * encoderMinimumBytesPerFrame - 1;

	if (frameSentToOpus > 0) {
		int32 uint8sizeBufferPCMMic = frameSentToOpus * encoderMinimumBytesPerFrame;
		
		TArray<int16> arrayToSend;
		arrayToSend.Insert(VoiceCaptureBufferAppended.GetData(), uint8sizeBufferPCMMic / 2, 0);
		if (callbackSpeakMicrophone != NULL && callbackSpeakMicrophone->IsValidLowLevel() && !callbackSpeakMicrophone->IsPendingKill()) {
			callbackSpeakMicrophone->receiveDataMicrophone(arrayToSend, arrayToSend.Num());
		}		
		//callbackSpeakMicrophone->receiveDataMicrophone(buffpass, InBufferFrames);
		// remove used data
		VoiceCaptureBufferAppended.RemoveAt(0, uint8sizeBufferPCMMic / 2, false);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OnAudioCapture frameSentToOpus %d %d %d"), frameSentToOpus, InBufferFrames, arrayToSend.Num()));

	}

	
}

bool FAudioCaptureIOS::GetInputDevicesAvailable(TArray<Audio::FCaptureDeviceInfo>& OutDevices)
{
	// TODO: Add individual devices for different ports here.
	OutDevices.Reset();

	Audio::FCaptureDeviceInfo& DeviceInfo = OutDevices.AddDefaulted_GetRef();
	GetCaptureDeviceInfo(DeviceInfo, 0);

	return true;
}

void FAudioCaptureIOS::SetHardwareFeatureEnabled(Audio::EHardwareInputFeature FeatureType, bool bEnabled)
{
	if (IOUnit == nil)
	{
		return;
	}

	// we ignore result those functions because sometime we can't set this parameters
	OSStatus status = noErr;

	switch (FeatureType)
	{
	case Audio::EHardwareInputFeature::EchoCancellation:
	{
		const UInt32 EnableParam = (bEnabled ? 0 : 1);
		status = AudioUnitSetProperty(IOUnit,
			kAUVoiceIOProperty_BypassVoiceProcessing,
			kAudioUnitScope_Global,
			kInputBus,
			&EnableParam,
			sizeof(EnableParam)
		);
	}
	break;
	case Audio::EHardwareInputFeature::AutomaticGainControl:
	{
		const UInt32 EnableParam = (bEnabled ? 1 : 0);
		status = AudioUnitSetProperty(IOUnit,
			kAUVoiceIOProperty_VoiceProcessingEnableAGC,
			kAudioUnitScope_Global,
			kInputBus,
			&EnableParam,
			sizeof(EnableParam)
		);
	}
	break;
	}
}
#endif