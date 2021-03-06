// Copyright 2020 Sabre Dart Studios


#include "ZSOculusPawn.h"

#include "MotionControllerComponent.h"
#include "OculusFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "zSpace/VR/VirtualkeyboarActor/VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"
#include "zSpace/VR/BallisticLineComponent/BallisticLineComponent.h"

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

	BallisticLineComponentLeft  = CreateDefaultSubobject<UBallisticLineComponent>(TEXT("BallisticLineComponentLeft"));
	checkf(nullptr != BallisticLineComponentLeft, TEXT("The BallisticLineComponentLeft is nullptr.") );
	BallisticLineComponentLeft->SetupAttachment(MotionControllerComponentLeft);
	
	BallisticLineComponentRight  = CreateDefaultSubobject<UBallisticLineComponent>(TEXT("BallisticLineComponentRight"));
	checkf(nullptr != BallisticLineComponentRight, TEXT("The BallisticLineComponentRight is nullptr.") );
	BallisticLineComponentRight->SetupAttachment(MotionControllerComponentRight);

	
}

// Called when the game starts or when spawned
void AZSOculusPawn::BeginPlay()
{
	Super::BeginPlay();
	OpenVRLevel();	
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
		PlayerInputComponent->BindAction(TEXT("OculusLTeleport"), EInputEvent::IE_Pressed, this, &AZSOculusPawn::OculusLTeleportPressed);
		PlayerInputComponent->BindAction(TEXT("OculusLTeleport"), EInputEvent::IE_Released, this, &AZSOculusPawn::OculusLTeleportReleased);
		PlayerInputComponent->BindAction(TEXT("OculusRTeleport"), EInputEvent::IE_Pressed, this, &AZSOculusPawn::OculusRTeleportPressed);
		PlayerInputComponent->BindAction(TEXT("OculusRTeleport"), EInputEvent::IE_Released, this, &AZSOculusPawn::OculusRTeleportReleased);
		
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

void AZSOculusPawn::OculusLTeleportPressed()
{
	BallisticLineComponentLeft->ShowBallisticLine();
}

void AZSOculusPawn::OculusLTeleportReleased()
{
	BallisticLineComponentLeft->HideBallisticLine();
}

void AZSOculusPawn::OculusRTeleportPressed()
{
	BallisticLineComponentRight->ShowBallisticLine();
}

void AZSOculusPawn::OculusRTeleportReleased()
{
	BallisticLineComponentRight->HideBallisticLine();
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

void AZSOculusPawn::OpenVRLevel()
{
	const EOculusDeviceType L_OculusDeviceType = UOculusFunctionLibrary::GetDeviceType();
	if(EOculusDeviceType::Quest_Link == L_OculusDeviceType)
	{
		const FString L_CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
		UWorld * L_World = WorldObjectPtrLogin.LoadSynchronous();
		if(L_World )
		{
			const FString L_LevelName = L_World->GetName();
			if(L_CurrentLevelName.Equals(L_LevelName))
			{
				UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), WorldObjectPtrVR);
			}
		}
	}
}

