// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "zSpace/Types/UITypes.h"

#include "PreviewCharacter.generated.h"

UCLASS()
class ZSPACE_API APreviewCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewCharacter();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* Root = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USceneCaptureComponent2D* SceneCaptureComponent2D = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* PreviewCharacterMeshComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class URotatingMovementComponent* RotatingMovementComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCharacterMeshesDataAsset* CharacterMeshesDataAsset = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class URenderTargetAndPosition* RenderTargetAndPosition = nullptr;

	int32 CurrentCharacterMeshIndex = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPreviewCharacterPosition PreviewCharacterPosition;

public:
	UFUNCTION(BlueprintCallable)
	void NextCharacterMesh();
	
	UFUNCTION(BlueprintCallable)
	void PreviousCharacterMesh();
	
	UFUNCTION(BlueprintPure)
	class UCharacterMeshesDataAsset* GetCharacterMeshesDataAsset() const;

	UFUNCTION(BlueprintPure)
	FName GetCurrentMeshName() const;
	
	UFUNCTION(BlueprintPure)
	class USkeletalMesh* GetCurrentMesh() const;	
};
