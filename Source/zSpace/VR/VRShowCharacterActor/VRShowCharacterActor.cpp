// Copyright 2020 Sabre Dart Studios


#include "VRShowCharacterActor.h"

#include "Kismet/GameplayStatics.h"
#include "zSpace/PlayerController/ZSLoginPlayerController.h"

// Sets default values
AVRShowCharacterActor::AVRShowCharacterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	checkf(nullptr != RootSceneComponent, TEXT("The RootSceneComponent is nullptr."));
	RootComponent = RootSceneComponent;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	checkf(nullptr != SkeletalMeshComponent, TEXT("The SkeletalMeshComponent is nullptr."));
	SkeletalMeshComponent->SetupAttachment(RootSceneComponent);

	
	
}

// Called when the game starts or when spawned
void AVRShowCharacterActor::BeginPlay()
{
	Super::BeginPlay();
	BindGetAllCharacter();	
	
}

// Called every frame
void AVRShowCharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRShowCharacterActor::BindGetAllCharacter()
{
	AZSLoginPlayerController * PC = Cast<AZSLoginPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(IsValid(PC))
	{
		PC->OnGetAllCharacters.AddUniqueDynamic(this, &AVRShowCharacterActor::OnGetAllCharacters);
	}
}

void AVRShowCharacterActor::OnGetAllCharacters(const TArray<FUserCharacter>& NewUserCharacters)
{
	UE_LOG(LogTemp, Log, TEXT("The UserCharactes Num = %d"), NewUserCharacters.Num());	
}

