// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Net/VoiceConfig.h"
#include "ZSVOIPTalker.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="ZSpace", meta = (BlueprintSpawnableComponent) )
class ZSPACE_API UZSVOIPTalker : public UVOIPTalker
{
	GENERATED_BODY()

public:


	virtual void OnTalkingBegin(UAudioComponent* NewAudioComponent) override;
	
	virtual void OnTalkingEnd() override;
};

