// Copyright 2020 Sabre Dart Studios


#include "zSpace//VOIPTalker/ZSVOIPTalker.h"

void UZSVOIPTalker::OnTalkingBegin(UAudioComponent* NewAudioComponent)
{
	Super::OnTalkingBegin(NewAudioComponent);
	UE_LOG(LogTemp, Log, TEXT("VOIP: before if *************** OnTalkingBegin ************************"));
	if(NewAudioComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("VOIP: in if *************** OnTalkingBegin ************************"));
		NewAudioComponent->Play();
	}
}

void UZSVOIPTalker::OnTalkingEnd()
{
	UE_LOG(LogTemp, Log, TEXT("VOIP: *************** OnTalkingEnd ************************"));
	Super::OnTalkingEnd();	
}
