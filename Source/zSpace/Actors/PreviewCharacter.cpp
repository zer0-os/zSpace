// Copyright 2020 Sabre Dart Studios


#include "zSpace/Actors/PreviewCharacter.h"

#include "zSpace/Types/CharacterMeshesDataAsset.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
APreviewCharacter::APreviewCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
	
	PreviewCharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("PreviewCharacterMesh");
	PreviewCharacterMeshComponent->SetupAttachment(RootComponent);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovementComponent");
}

void APreviewCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(RenderTargetAndPosition))
	{
		auto Data =  RenderTargetAndPosition->RenderTargetAndPosition;
		UTextureRenderTarget2D* const* RenderTarget = Data.Find(PreviewCharacterPosition);
		if (RenderTarget)
		{
			SceneCaptureComponent2D->TextureTarget = *RenderTarget;
		}	
	}
}

// Called when the game starts or when spawned
void APreviewCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APreviewCharacter::NextCharacterMesh()
{
	TArray<USkeletalMesh*> Meshes = CharacterMeshesDataAsset->GetAllSkeletalMeshes();

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
}

void APreviewCharacter::PreviousCharacterMesh()
{
	TArray<USkeletalMesh*> Meshes = CharacterMeshesDataAsset->GetAllSkeletalMeshes();

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
}

UCharacterMeshesDataAsset* APreviewCharacter::GetCharacterMeshesDataAsset() const
{
	return CharacterMeshesDataAsset;
}

FName APreviewCharacter::GetCurrentMeshName() const
{
	TArray<FName> Names;

	CharacterMeshesDataAsset->GetAllCharacterNames(Names);

	if (Names.IsValidIndex(CurrentCharacterMeshIndex))
	{
		return Names[CurrentCharacterMeshIndex];
	}
	return NAME_None;
}

USkeletalMesh* APreviewCharacter::GetCurrentMesh() const
{
	TArray<USkeletalMesh*> Meshes = CharacterMeshesDataAsset->GetAllSkeletalMeshes();

	if (Meshes.IsValidIndex(CurrentCharacterMeshIndex))
	{
		return Meshes[CurrentCharacterMeshIndex];
	}
	
	return nullptr;
}
