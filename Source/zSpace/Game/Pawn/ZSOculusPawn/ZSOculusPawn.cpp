// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/Pawn/ZSOculusPawn/ZSOculusPawn.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Widget.h"
#include "Components/WidgetInteractionComponent.h"

// Sets default values
AZSOculusPawn::AZSOculusPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	checkf(nullptr != RootSceneComponent, TEXT("The RootSceneComponent is nullptr."));
	RootComponent = RootSceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	checkf(nullptr != CameraComponent, TEXT("The CameraComponent is nullptr."));
	CameraComponent->SetupAttachment(RootComponent);

	MotionControllerComponentLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentLeft"));
	checkf(nullptr != MotionControllerComponentLeft, TEXT("The MotionControllerComponentLeft is nullptr."));
	MotionControllerComponentLeft->SetupAttachment(RootComponent);
	
	MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentRight"));
	checkf(nullptr != MotionControllerComponentRight, TEXT("The MotionControllerComponentRight is nullptr."));
	MotionControllerComponentRight->SetupAttachment(RootComponent);

	SkeletalMeshComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentLeft"));
	checkf(nullptr != SkeletalMeshComponentLeft, TEXT("The SkeletalMeshComponentLeft is nullptr.") );
	SkeletalMeshComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	SkeletalMeshComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentRight"));
	checkf(nullptr != SkeletalMeshComponentRight, TEXT("The SkeletalMeshComponentRight is nullptr."));
	SkeletalMeshComponentRight->SetupAttachment(MotionControllerComponentRight);

	WidgetInteractionComponentLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentLeft"));
	checkf(nullptr != WidgetInteractionComponentLeft, TEXT("The WidgetInteractionComponentLeft is nullptr."));
	WidgetInteractionComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	WidgetInteractionComponentRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentRight"));
	checkf(nullptr != WidgetInteractionComponentRight, TEXT("The WidgetInteractionComponentRight is nullptr."));
	WidgetInteractionComponentRight->SetupAttachment(MotionControllerComponentRight);
	

}

// Called when the game starts or when spawned
void AZSOculusPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZSOculusPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZSOculusPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

