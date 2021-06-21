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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UBallisticLineComponent * BallisticLineComponentLeft = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UBallisticLineComponent * BallisticLineComponentRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UWorld > WorldObjectPtr;

public:

	// input Start

	UFUNCTION()
	void OculusXPressed();

	UFUNCTION()
	void OculusXReleased();

	UFUNCTION()
	void OculusAPressed();
		
	UFUNCTION()
	void OculusAReleased();

	UFUNCTION()
	void OculusLTeleportPressed();

	
	UFUNCTION()
	void OculusLTeleportReleased();
	
	UFUNCTION()
	void OculusRTeleportPressed();
	
	UFUNCTION()
	void OculusRTeleportReleased();
	
	// Input End

	UFUNCTION()
	void HoveredWidgetChanged(class UWidgetComponent* NewWidgetComponent, class UWidgetComponent* NewPreviousWidgetComponent);

	void OpenVRLevel();

	
};
