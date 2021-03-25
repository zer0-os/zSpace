// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.
#include "PlayerVoiceChatActor.h"




/* singleton local joueur */
APlayerVoiceChatActor *APlayerVoiceChatActor::myPlayerVoiceActor = NULL;


APlayerVoiceChatActor *UUniversalVoiceChat::GetMyPlayerVoiceActor() {
	return APlayerVoiceChatActor::myPlayerVoiceActor;
}



APlayerVoiceChatActor::APlayerVoiceChatActor() : Super() {
	bReplicates = true;
	bNetLoadOnClient = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	SetActorTickInterval(0.2f);
	//root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

}


// server add mic comp on start
void APlayerVoiceChatActor::BeginPlay() {
	Super::BeginPlay();

	// spawn micro comp par server
	if (GetWorld()->IsServer()) {
		microphoneSpeakComponent = NewObject<UMicrophoneSpeakComponent>(this, UMicrophoneSpeakComponent::StaticClass());		
		microphoneSpeakComponent->RegisterComponent();		
		microphoneSpeakComponent->SetIsReplicated(true);
		if (GetWorld()->GetNetMode() == NM_Standalone) {
			SetOwner(UGameplayStatics::GetPlayerController(this, 0));
			OnRep_Owner(); // not called in standalone, so call it manually
		}

		if (GetWorld()->GetNetMode() == NM_ListenServer) {
			bool OwnedLocally = IsOwnedBy(UGameplayStatics::GetPlayerController(this, 0));
			if (OwnedLocally) {
				UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnRep_Owner OwnedLocally server %d"), GetWorld()->IsServer());
				myPlayerVoiceActor = this;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnRep_Owner not OwnedLocally server %d"), GetWorld()->IsServer());
			}
		}

		
		if (this->microphoneSpeakComponent) {
			UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor BeginPlay try set attenuation"));
			if (!pathToAttenuationAsset.IsEmpty()) {
				this->microphoneSpeakComponent->setAttenuationAssetPath(true, pathToAttenuationAsset);
			}
			else {
				this->microphoneSpeakComponent->setAttenuationAssetPath(false, pathToAttenuationAsset);
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor BeginPlay try set attenuation his->microphoneSpeakComponent null"));
		}

		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "DelegateEndPlayOwner");
		GetOwner()->OnEndPlay.AddUnique(Delegate);

		
		if (GetOwner()->IsA(APlayerController::StaticClass())) {
			APlayerController* controllerOwner = ((APlayerController*)GetOwner());
			ownerPlayerState = controllerOwner->GetPlayerState<APlayerState>();
		}
	}	
}

// clean static if owned locally on destroy
void APlayerVoiceChatActor::EndPlay(const EEndPlayReason::Type EndPlayReason){

	Super::EndPlay(EndPlayReason);
	//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnDestroy"));
	bool OwnedLocally = myPlayerVoiceActor == this;
	if (OwnedLocally) {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor EndPlay OwnedLocally server %d"), GetWorld()->IsServer());
		// make sure to cut audio
		UUniversalVoiceChat::VoiceChatStopSpeak();
		// NULL the singleton
		myPlayerVoiceActor = NULL;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor EndPlay not OwnedLocally server %d"), GetWorld()->IsServer());
	}


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerVoiceChatActor::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor EndPlay total actodleft %d"), FoundActors.Num());
}

// replication UE4 interne
void APlayerVoiceChatActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerVoiceChatActor, microphoneSpeakComponent);
	DOREPLIFETIME(APlayerVoiceChatActor, idVoiceChat);
	DOREPLIFETIME(APlayerVoiceChatActor, ownerPlayerState);
	DOREPLIFETIME(APlayerVoiceChatActor, radioChannelSubscribed);
	DOREPLIFETIME(APlayerVoiceChatActor, pathToAttenuationAsset);
	DOREPLIFETIME(APlayerVoiceChatActor, voiceVolume);
	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor GetLifetimeReplicatedProps"));
}

void APlayerVoiceChatActor::RepNotifyMicComp() {

	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor::RepNotifyMicComp"));
	if (this->microphoneSpeakComponent) {
		if (!pathToAttenuationAsset.IsEmpty()) {
			this->microphoneSpeakComponent->setAttenuationAssetPath(true, pathToAttenuationAsset);
		}
		else {
			this->microphoneSpeakComponent->setAttenuationAssetPath(false, pathToAttenuationAsset);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor RepNotifyMicComp his->microphoneSpeakComponent null"));
	}
}

void APlayerVoiceChatActor::RepNotifyAttenuationAsset() {
	
	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor::RepNotifyAttenuationAsset"));
	//if (!pathToAttenuationAsset.IsEmpty()) {
	//	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor RepNotifyAttenuationAsset server ? %d %s"), GetWorld()->IsServer(), *pathToAttenuationAsset);
	//}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RepNotifyAttenuationAsset %s empty? %d"), *GetName(), pathToAttenuationAsset.IsEmpty()));

	if (this->microphoneSpeakComponent) {
		if (!pathToAttenuationAsset.IsEmpty()) {
			this->microphoneSpeakComponent->setAttenuationAssetPath(true, pathToAttenuationAsset);
		}
		else {
			this->microphoneSpeakComponent->setAttenuationAssetPath(false, pathToAttenuationAsset);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor RepNotifyAttenuationAsset his->microphoneSpeakComponent null"));
	}	
}

void APlayerVoiceChatActor::RepNotifyVoiceVolume() {
	UE_LOG(LogTemp, Warning, TEXT("RepNotifyVoiceVolume"));
	if (this->microphoneSpeakComponent) {
		this->microphoneSpeakComponent->SetVoiceVolume(voiceVolume);
	}
}

// automatically remove voice chat if owner is destroyed
void APlayerVoiceChatActor::DelegateEndPlayOwner(AActor* act, EEndPlayReason::Type EndPlayReason) {
	UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor DelegateEndPlayOwner"));
	if (!IsPendingKill()) {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor DelegateEndPlayOwner start destroy this"));
		Destroy();
	}
}

// event called when server has set owner
void APlayerVoiceChatActor::OnRep_Owner() {
	Super::OnRep_Owner();
	//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnRep_Owner"));
	
	bool OwnedLocally = IsOwnedBy(UGameplayStatics::GetPlayerController(this, 0)) && GetNetMode() != NM_DedicatedServer;
	if (OwnedLocally) {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnRep_Owner OwnedLocally server %d"), GetWorld()->IsServer());
		myPlayerVoiceActor = this;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor OnRep_Owner not OwnedLocally server %d"), GetWorld()->IsServer());
	}
}


void APlayerVoiceChatActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor server ? %d ownerPlayerState NULL ? %d "), GetWorld()->IsServer(), ownerPlayerState == NULL);


	if (GetWorld() != NULL && GetWorld()->IsServer()) {
		APlayerController *ownerPC = (APlayerController *)GetOwner();
		if (ownerPC != NULL) {
			if (ownerPC->GetPawn() != NULL) {
				SetActorLocation(ownerPC->GetPawn()->GetActorLocation());
				RPCServerUpdatePosAudioComp(GetActorLocation());
				//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor  GetActorLocation %s"), *(ownerPC->GetCharacter()->GetActorLocation().ToString()) );
			}
		}
	}
	
}


/* server set, pathToAttenuationAsset is automatically replicated */
void APlayerVoiceChatActor::ServerSetAttenuation(bool enableAttenuation, FString _pathToAttenuationAsset) {
	if (enableAttenuation) {
		pathToAttenuationAsset = _pathToAttenuationAsset;
	}
	else {
		pathToAttenuationAsset = "";
	}
}


void APlayerVoiceChatActor::ServerSetAllowUseProximity(bool _allowUseRange) {
	AntiCheatAllowUseProximity = _allowUseRange;
}



void APlayerVoiceChatActor::ServerSetAllowUseGlobal(bool _allowUseGlobal) {
	AntiCheatAllowUseGlobal = _allowUseGlobal;
}



void  APlayerVoiceChatActor::ServerSetMaxProximityRange(float _maxProximityRange) {
	AntiCheatMaxProximityRange = _maxProximityRange;
}



/* mute audio locally, no server involved in this */
void APlayerVoiceChatActor::muteAudio(bool isMute) {
	if (microphoneSpeakComponent != NULL) {
		microphoneSpeakComponent->muteAudio(isMute);
	}	
}

/* server set, radioChannelSubscribed is automatically replicated */
void APlayerVoiceChatActor::ServerAddChannel(int32 channelToAdd) {
	if (!radioChannelSubscribed.Contains(channelToAdd)) {
		radioChannelSubscribed.Add(channelToAdd);
	}
	//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor::AddChannel total %d"), radioChannelSubscribed.Num());
}

/* server set, radioChannelSubscribed is automatically replicated */
void APlayerVoiceChatActor::ServerRemoveChannel(int32 channelToRemove) {
	radioChannelSubscribed.Remove(channelToRemove);
	//UE_LOG(LogTemp, Warning, TEXT("APlayerVoiceChatActor::RemoveChannel total %d"), radioChannelSubscribed.Num());
}

/* RPC Server => Client */
void APlayerVoiceChatActor::RPCServerUpdatePosAudioComp_Implementation(FVector worldPos) {
	SetActorLocation(worldPos);
}

/* RPC Client => Server */
void APlayerVoiceChatActor::RPCClientAskAddChannel_Implementation(int32 channelToAdd) {
	ServerAddChannel(channelToAdd);
}

/* RPC Client => Server */
void APlayerVoiceChatActor::RPCClientAskRemoveChannel_Implementation(int32 channelToRemove) {
	ServerRemoveChannel(channelToRemove);
}


void APlayerVoiceChatActor::RPCClientSetMicrophoneVolume_Implementation(float volume) {
	voiceVolume = volume;
	if (microphoneSpeakComponent != NULL) {
		microphoneSpeakComponent->SetVoiceVolume(volume);
	}
}


/************** BLUEPRINT LIBRARY ***************************************/


// get your local player voice chat, you need this if you want to receive raw microphone data
APlayerVoiceChatActor* UUniversalVoiceChat::VoiceChatGetMyLocalPlayerVoiceChat() {
	if (APlayerVoiceChatActor::myPlayerVoiceActor == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("UUniversalVoiceChat::GetMyLocalPlayerVoiceChat is NULL"));
	}
	return APlayerVoiceChatActor::myPlayerVoiceActor;
}

bool UUniversalVoiceChat::VoiceChatInitAudioVoiceChatQuality(int32 _sampleRate, int32 _numChannels) {
	if (APlayerVoiceChatActor::myPlayerVoiceActor == NULL || APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("UUniversalVoiceChat::initVoiceChatQuality myPlayerVoiceActor is NULL"));
		return false;
	}
	return APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent->initAudioResources(_sampleRate, _numChannels);
}

bool UUniversalVoiceChat::VoiceChatWasInitAudioVoiceChatQuality() {
	if (APlayerVoiceChatActor::myPlayerVoiceActor == NULL || APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("UUniversalVoiceChat::wasInitAudioVoiceChatQuality is NULL"));
		return false;
	}
	return APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent->getWasInitAudioResources();
}

// use this to start speaking (local owned client side only)
bool UUniversalVoiceChat::VoiceChatStartSpeak(bool _shouldHearMyOwnVoice, bool isGlobal, int radioChannel, bool useRange, float maxRange) {

	bool res = false;

	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatStartSpeak ok _shouldHearMyOwnVoice %d  isGlobal %d radioChannel %d"), _shouldHearMyOwnVoice, isGlobal, radioChannel);
		res = APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent->startSpeaking(_shouldHearMyOwnVoice, isGlobal, radioChannel, useRange, maxRange);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatStartSpeak error"));
		res = false;
	}
	return res;
}

// use this to stop speaking (local owned client side only)
bool UUniversalVoiceChat::VoiceChatStopSpeak() {

	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatStopSpeak ok"));
		APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent->endSpeaking();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatStopSpeak error"));
		return false;
	}
	return true;
}

bool UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(float volume) {

	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatSetMicrophoneVolume ok"));
		APlayerVoiceChatActor::myPlayerVoiceActor->RPCClientSetMicrophoneVolume(volume);

		if (APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent != NULL) {
			APlayerVoiceChatActor::myPlayerVoiceActor->microphoneSpeakComponent->SetVoiceVolume(volume);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatSetMicrophoneVolume error"));
		return false;
	}
	return true;
}



// use this to mute someone (local owned client side only)
void UUniversalVoiceChat::VoiceChatLocalMuteSomeone(const UObject* WorldContextObject, APlayerState *playerToMute, bool shouldMute) {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), APlayerVoiceChatActor::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT(" VoiceChatMuteSomeone total actor found %d"), FoundActors.Num());
	for (int i = 0; i < FoundActors.Num(); i++) {
		if (((APlayerVoiceChatActor*)FoundActors[i])->ownerPlayerState == playerToMute) {
			((APlayerVoiceChatActor*)FoundActors[i])->muteAudio(shouldMute);
			break;
		}
	}
}

// check if we muted someone locally (local owned client side only)
bool UUniversalVoiceChat::VoiceChatLocalIsMutedSomeone(const UObject* WorldContextObject, APlayerState *playerToCheckMute) {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), APlayerVoiceChatActor::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT(" VoiceChatLocalIsMutedSomeone total actor found %d"), FoundActors.Num());
	for (int i = 0; i < FoundActors.Num(); i++) {
		if (((APlayerVoiceChatActor*)FoundActors[i])->ownerPlayerState == playerToCheckMute && ((APlayerVoiceChatActor*)FoundActors[i])->microphoneSpeakComponent) {
			return ((APlayerVoiceChatActor*)FoundActors[i])->microphoneSpeakComponent->isMutedLocalSetting;
		}
	}
	return false;
}


bool  UUniversalVoiceChat::VoiceChatCheckRegisteredToChannel(int32 channelToCheck) {
	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatCheckRegisteredToChannel ok"));
		return APlayerVoiceChatActor::myPlayerVoiceActor->radioChannelSubscribed.Contains(channelToCheck);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatCheckRegisteredToChannel error"));
		return false;
	}
}


bool UUniversalVoiceChat::VoiceChatAddChannel(int32 channelToAdd) {
	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatAddChannel ok"));
		APlayerVoiceChatActor::myPlayerVoiceActor->RPCClientAskAddChannel(channelToAdd);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatAddChannel error"));
		return false;
	}
	return true;
}

bool UUniversalVoiceChat::VoiceChatRemoveChannel(int32 channelToRemove) {
	if (APlayerVoiceChatActor::myPlayerVoiceActor != NULL && IsValid(APlayerVoiceChatActor::myPlayerVoiceActor)) {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatRemoveChannel ok"));
		APlayerVoiceChatActor::myPlayerVoiceActor->RPCClientAskRemoveChannel(channelToRemove);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VoiceChatRemoveChannel error"));
		return false;
	}
	return true;
}

bool  UUniversalVoiceChat::VoiceChatHasMicrophonePermission() {

#if PLATFORM_IOS
	bool res = false;
	switch ([[AVAudioSession sharedInstance] recordPermission]) {
		case AVAudioSessionRecordPermissionGranted:
			res = true;
			break;
		case AVAudioSessionRecordPermissionDenied:
			res = false;
			break;
		case AVAudioSessionRecordPermissionUndetermined:
			res = false;
			break;
		default:
			res = false;
			break;
	}
	return res;
#endif
#if PLATFORM_ANDROID
	return UAudioCaptureAndroid::AndroidHasPermission();
#endif
#if PLATFORM_WINDOWS	
	return true;
#endif
	return true;
}

void  UUniversalVoiceChat::VoiceChatAskMicrophonePermission() {
#if PLATFORM_IOS
	[[AVAudioSession sharedInstance] requestRecordPermission:^ (BOOL granted) {
			if (granted)
			{

			}
			else
			{

			}

	}];

#endif
#if PLATFORM_ANDROID
	UAudioCaptureAndroid::AndroidAskPermission();
#endif
#if PLATFORM_WINDOWS	
	
#endif
}
