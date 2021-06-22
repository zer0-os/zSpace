// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelsoundsPlayer.generated.h"

UCLASS()
class ZSPACE_API ALevelsoundsPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelsoundsPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class ULevelsSoundsDataAsset* LevelsSoundsDatasset;

	UFUNCTION(BlueprintCallable)
	void PlayLevelSounds();

};
