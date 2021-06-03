// Copyright 2020 Sabre Dart Studios


#include "zSpace/Actors/LevelsoundsPlayer.h"
#include "../Types/LevelsSoundsDataAsset.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include "UObject/SoftObjectPtr.h"


// Sets default values
ALevelsoundsPlayer::ALevelsoundsPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}

// Called when the game starts or when spawned
void ALevelsoundsPlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayLevelSounds();
	
}

// Called every frame
void ALevelsoundsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelsoundsPlayer::PlayLevelSounds()
{
	if (LevelsSoundsDatasset)
	{
		
		FName LevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
		TArray<TSoftObjectPtr<USoundBase>>  LevelSounds = LevelsSoundsDatasset->GetSoundsByLevelName(LevelName);

		for (TSoftObjectPtr<USoundBase> IterSound : LevelSounds)
		{
			USoundBase  * L_Sound = Cast<USoundBase>(IterSound.LoadSynchronous());
			UGameplayStatics::PlaySound2D(this, L_Sound);
		}
	}

}

