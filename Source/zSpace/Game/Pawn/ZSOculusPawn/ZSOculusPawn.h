// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ZSOculusPawn.generated.h"

UCLASS()
class ZSPACE_API AZSOculusPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZSOculusPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USceneComponent * RootSceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UCameraComponent * CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UMotionControllerComponent * MotionControllerComponentLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UMotionControllerComponent * MotionControllerComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USkeletalMeshComponent * SkeletalMeshComponentLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USkeletalMeshComponent * SkeletalMeshComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponentLeft = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UWidgetInteractionComponent * WidgetInteractionComponentRight = nullptr;


};
