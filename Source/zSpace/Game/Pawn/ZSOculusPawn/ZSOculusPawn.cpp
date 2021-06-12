// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/Pawn/ZSOculusPawn/ZSOculusPawn.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "zSpace/VirtualkeyboarActor/VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"

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
	MotionControllerComponentLeft->MotionSource = TEXT("Left");
	
	MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentRight"));
	checkf(nullptr != MotionControllerComponentRight, TEXT("The MotionControllerComponentRight is nullptr."));
	MotionControllerComponentRight->SetupAttachment(RootComponent);
	MotionControllerComponentRight->MotionSource = TEXT("Right");

	SkeletalMeshComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentLeft"));
	checkf(nullptr != SkeletalMeshComponentLeft, TEXT("The SkeletalMeshComponentLeft is nullptr.") );
	SkeletalMeshComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	SkeletalMeshComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponentRight"));
	checkf(nullptr != SkeletalMeshComponentRight, TEXT("The SkeletalMeshComponentRight is nullptr."));
	SkeletalMeshComponentRight->SetupAttachment(MotionControllerComponentRight);

	WidgetInteractionComponentLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentLeft"));
	checkf(nullptr != WidgetInteractionComponentLeft, TEXT("The WidgetInteractionComponentLeft is nullptr."));
	WidgetInteractionComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	WidgetInteractionComponentLeft->bShowDebug = true;
	WidgetInteractionComponentLeft->InteractionDistance = 1000;
	WidgetInteractionComponentLeft->PointerIndex = 0;
	WidgetInteractionComponentLeft->VirtualUserIndex = 0;
	
	WidgetInteractionComponentRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponentRight"));
	checkf(nullptr != WidgetInteractionComponentRight, TEXT("The WidgetInteractionComponentRight is nullptr."));
	WidgetInteractionComponentRight->SetupAttachment(MotionControllerComponentRight);
	WidgetInteractionComponentRight->bShowDebug = true;
	WidgetInteractionComponentRight->InteractionDistance = 1000;
	WidgetInteractionComponentRight->PointerIndex = 1;
	WidgetInteractionComponentRight->VirtualUserIndex = 1;

	WidgetInteractionComponentRight->OnHoveredWidgetChanged.AddUniqueDynamic(this, &AZSOculusPawn::HoveredWidgetChanged);

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
	if(IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAction(TEXT("OculusXPress"), EInputEvent::IE_Pressed, this, &AZSOculusPawn::OculusXPressed);
		PlayerInputComponent->BindAction(TEXT("OculusXPress"), EInputEvent::IE_Released, this, &AZSOculusPawn::OculusXReleased);
		PlayerInputComponent->BindAction(TEXT("OculusAPress"), EInputEvent::IE_Pressed, this, &AZSOculusPawn::OculusAPressed);
		PlayerInputComponent->BindAction(TEXT("OculusAPress"), EInputEvent::IE_Released, this, &AZSOculusPawn::OculusAReleased);
	}

}

void AZSOculusPawn::OculusXPressed()
{
	checkf(nullptr != WidgetInteractionComponentLeft, TEXT("The WidgetInteractionComponentLeft is nullptr."));
	if(IsValid(WidgetInteractionComponentLeft))
	{
		const FKey L_Key(EKeys::LeftMouseButton);
		WidgetInteractionComponentLeft->PressPointerKey(L_Key);
	}
}

void AZSOculusPawn::OculusXReleased()
{
	checkf(nullptr != WidgetInteractionComponentLeft, TEXT("The WidgetInteractionComponentLeft is nullptr."));
	if(IsValid(WidgetInteractionComponentLeft))
	{
		const FKey L_Key(EKeys::LeftMouseButton);
		WidgetInteractionComponentLeft->ReleasePointerKey(L_Key);
	}
	
}

void AZSOculusPawn::OculusAPressed()
{
	checkf(nullptr != WidgetInteractionComponentRight, TEXT("The WidgetInteractionComponentRight is nullptr."));
	if(IsValid(WidgetInteractionComponentRight))
	{
		const FKey L_Key(EKeys::LeftMouseButton);
		WidgetInteractionComponentRight->PressPointerKey(L_Key);
	}
}

void AZSOculusPawn::OculusAReleased()
{
	checkf(nullptr != WidgetInteractionComponentRight, TEXT("The WidgetInteractionComponentRight is nullptr."));
	if(IsValid(WidgetInteractionComponentRight))
	{
		const FKey L_Key(EKeys::LeftMouseButton);
		WidgetInteractionComponentRight->ReleasePointerKey(L_Key);
	}
}

void AZSOculusPawn::HoveredWidgetChanged(UWidgetComponent * NewWidgetComponent, UWidgetComponent * NewPreviousWidgetComponent)
{
	if(IsValid(NewWidgetComponent) && IsValid(WidgetInteractionComponentRight))
	{
		UUserWidget * L_UserWidgetObject = NewWidgetComponent->GetUserWidgetObject();
		if(IsValid(L_UserWidgetObject))
		{
			const bool L_IsImplemented = L_UserWidgetObject->GetClass()->ImplementsInterface(UVirtualKeyboardWidgetInterface::StaticClass());
			if(L_IsImplemented)
			{
				IVirtualKeyboardWidgetInterface::Execute_SetWidgetInteractionComponent(L_UserWidgetObject, WidgetInteractionComponentRight);	
			}
		}
	}
}

