// Copyright 2020 Sabre Dart Studios


#include "zSpace/VirtualkeyboarActor/VirtualKeyboardActor.h"
#include "Components/WidgetComponent.h"
#include "VirtualKeyboardWidgetInterface/VirtualKeyboardWidgetInterface.h"

// Sets default values
AVirtualKeyboardActor::AVirtualKeyboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	checkf(nullptr != RootSceneComponent, TEXT("The iRootSceneComponent is nullptr."));
	RootComponent = RootSceneComponent;

	WidgetComponentVirtualKeyboard = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponentVirtualKeyboard"));
	checkf(nullptr != WidgetComponentVirtualKeyboard, TEXT("The WidgetComponentVirtualKeyboard is nullptr. "));
	WidgetComponentVirtualKeyboard->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AVirtualKeyboardActor::BeginPlay()
{
	Super::BeginPlay();
	SetVirtualKeyboardActor();	
}

// Called every frame
void AVirtualKeyboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVirtualKeyboardActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AVirtualKeyboardActor::SetVirtualKeyboardActor()
{
	if(IsValid(WidgetComponentVirtualKeyboard))
	{
		UUserWidget * L_UserWidget = WidgetComponentVirtualKeyboard->GetUserWidgetObject();
		if(L_UserWidget)
		{
			const bool L_IsImplemented = L_UserWidget->GetClass()->ImplementsInterface(UVirtualKeyboardWidgetInterface::StaticClass());
			if(L_IsImplemented)
			{
				IVirtualKeyboardWidgetInterface::Execute_SetVirtualKeyboardActor(L_UserWidget, this);	
			}
		}
	}
}

void AVirtualKeyboardActor::SetIsPressedShift(const bool NewIsPressedShift)
{
	bIsPressedShift = NewIsPressedShift;
	OnPressedShift.Broadcast(bIsPressedShift);
}

