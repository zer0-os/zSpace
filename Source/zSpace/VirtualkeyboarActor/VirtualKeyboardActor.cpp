// Copyright 2020 Sabre Dart Studios


#include "zSpace/VirtualkeyboarActor/VirtualKeyboardActor.h"
#include "Components/WidgetComponent.h"

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
	
}

// Called every frame
void AVirtualKeyboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

