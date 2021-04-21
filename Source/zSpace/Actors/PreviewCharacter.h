// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"

#include "PreviewCharacter.generated.h"

UCLASS()
class ZSPACE_API APreviewCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* PreviewCharacterMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class URotatingMovementComponent* RotatingMovementComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCharacterMeshesDataAsset* CharacterMeshesDataAsset = nullptr;
	// TSoftObjectPtr<class UCharacterMeshesDataAsset> CharacterMeshesDataAsset;

	int32 CurrentCharacterMeshIndex = 0;

public:
	UFUNCTION(BlueprintCallable)
	void NextCharacterMesh();
	
	UFUNCTION(BlueprintCallable)
	void PreviousCharacterMesh();
};
