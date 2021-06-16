// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "OWSPlayerController.h"
#include "GameFramework/Actor.h"
#include "VRShowCharacterActor.generated.h"

UCLASS()
class ZSPACE_API AVRShowCharacterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRShowCharacterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USceneComponent * RootSceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USkeletalMeshComponent * SkeletalMeshComponent = nullptr;

	void BindGetAllCharacter();

	UFUNCTION()
	void OnGetAllCharacters(const TArray<FUserCharacter>& UserCharacters);

	
};
