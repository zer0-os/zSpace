// Copyright 2020 Sabre Dart Studios


#include "zSpace/Actors/PreviewCharacter.h"


#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"

// Sets default values
APreviewCharacter::APreviewCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PreviewCharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("PreviewCharacterMesh");
	SetRootComponent(PreviewCharacterMeshComponent);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovementComponent");
}

// Called when the game starts or when spawned
void APreviewCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APreviewCharacter::NextCharacterMesh()
{
	const TArray<USkeletalMesh*> Meshes = CharacterMeshesDataAsset->CharacterMeshes;
	if (CurrentCharacterMeshIndex + 1 > Meshes.Num() - 1)
	{
		CurrentCharacterMeshIndex = 0;
	}
	else
	{
		CurrentCharacterMeshIndex++;
	}

	if (Meshes.IsValidIndex(CurrentCharacterMeshIndex) && IsValid(Meshes[CurrentCharacterMeshIndex]))
	{
		PreviewCharacterMeshComponent->SetSkeletalMesh(Meshes[CurrentCharacterMeshIndex]);
	}

	// UKismetSystemLibrary::PrintString(this, FString::FromInt(CurrentCharacterMeshIndex));
}

void APreviewCharacter::PreviousCharacterMesh()
{
	const TArray<USkeletalMesh*> Meshes = CharacterMeshesDataAsset->CharacterMeshes;

	if (CurrentCharacterMeshIndex - 1 < 0)
	{
		CurrentCharacterMeshIndex = Meshes.Num() - 1;
	}
	else
	{
		CurrentCharacterMeshIndex--;
	}
	
	if (Meshes.IsValidIndex(CurrentCharacterMeshIndex) && IsValid(Meshes[CurrentCharacterMeshIndex]))
	{
		PreviewCharacterMeshComponent->SetSkeletalMesh(Meshes[CurrentCharacterMeshIndex]);
	}
	
	// UKismetSystemLibrary::PrintString(this, FString::FromInt(CurrentCharacterMeshIndex));
}
