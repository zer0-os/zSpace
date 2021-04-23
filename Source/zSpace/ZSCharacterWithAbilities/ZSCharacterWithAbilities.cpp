// Copyright 2020 Sabre Dart Studios


#include "ZSCharacterWithAbilities.h"

#include "OWSGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AZSCharacterWithAbilities::AZSCharacterWithAbilities(const FObjectInitializer& NewObjectInitializer) : Super(NewObjectInitializer)
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	checkf(nullptr != SpringArmComponent, TEXT("The SpringArmComponent is nullptr."));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->SetRelativeLocation(FVector(0.0,70,68));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	checkf(nullptr != CameraComponent, TEXT("The CameraComponent is nullptr."));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	DetectSurfaceTypeComponent = CreateDefaultSubobject<UDetectSurfaceTypeComponent>(TEXT("DetectSurfaceTypeComponent"));
	checkf(nullptr != DetectSurfaceTypeComponent, TEXT("The DetectSurfaceTypeComponent is nullptr."));
	AddOwnedComponent(DetectSurfaceTypeComponent);
	
}

void AZSCharacterWithAbilities::SetupPlayerInputComponent(UInputComponent* NewPlayerInputComponent)
{

	if(NewPlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(NewPlayerInputComponent);
		NewPlayerInputComponent->BindAxis(TEXT("Turn"), this, &AZSCharacterWithAbilities::Turn);
		NewPlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AZSCharacterWithAbilities::LookUp);
		NewPlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AZSCharacterWithAbilities::MoveForward);
		NewPlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AZSCharacterWithAbilities::MoveRight);
		
		NewPlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AZSCharacterWithAbilities::Jump);
		NewPlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AZSCharacterWithAbilities::StopJumping);
		NewPlayerInputComponent->BindAction(TEXT("ShowPlayersOnline"), IE_Pressed, this, &AZSCharacterWithAbilities::ShowPlayersOnline);
		NewPlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AZSCharacterWithAbilities::Dodge);
	}
}

void AZSCharacterWithAbilities::BeginPlay()
{
	Super::BeginPlay();
}

void AZSCharacterWithAbilities::Tick(float NewDeltaSeconds)
{
	Super::Tick(NewDeltaSeconds);
}

void AZSCharacterWithAbilities::Turn(float NewValue)
{
	if(false == IsTransferringBetweenMaps)
	{
		if(GetWorld())
		{
			const float L_DeltaSeconds = GetWorld()->GetDeltaSeconds();
			const float R_Input = NewValue * L_DeltaSeconds * BaseTurnRate;
			AddControllerYawInput(R_Input);
		}
	}
}

void AZSCharacterWithAbilities::LookUp(float NewValue)
{
	if(false == IsTransferringBetweenMaps)
	{
		if(GetWorld())
		{
			const float L_DeltaSeconds = GetWorld()->GetDeltaSeconds();
			const float R_Input = NewValue * L_DeltaSeconds * BaseLookUpRate;
			AddControllerPitchInput(R_Input);
		}
	}
}

void AZSCharacterWithAbilities::Jump()
{
	Super::Jump();
}

void AZSCharacterWithAbilities::StopJumping()
{
	Super::StopJumping();	
}

void AZSCharacterWithAbilities::MoveForward(float NewValue)
{
	UE_LOG(LogTemp, Log, TEXT("*************** The NewValue = %f *******************"), NewValue);
	// DataTable'/Game/AbilitySystem/Abilities/MyGameplayTagsTable.MyGameplayTagsTable'
	const FGameplayTag L_GameplayTag = FGameplayTag::RequestGameplayTag(FName("Combat.IsAttackingCannotMove"));
	const bool L_HasMatchingGameplayTag =   AbilitySystem ?  AbilitySystem->HasMatchingGameplayTag(L_GameplayTag) : false;
	if(false == IsTransferringBetweenMaps && false == L_HasMatchingGameplayTag)
	{
		const FQuat L_Quat(FRotator(0,GetControlRotation().Yaw, 0));
		const FVector R_ForwardVector = L_Quat.GetForwardVector();
		AddMovementInput(R_ForwardVector, NewValue);
	}
	
}

void AZSCharacterWithAbilities::MoveRight(float NewValue)
{
	// DataTable'/Game/AbilitySystem/Abilities/MyGameplayTagsTable.MyGameplayTagsTable'
	const FGameplayTag L_GameplayTag = FGameplayTag::RequestGameplayTag(FName("Combat.IsAttackingCannotMove"));
	const bool L_HasMatchingGameplayTag =   AbilitySystem ?  AbilitySystem->HasMatchingGameplayTag(L_GameplayTag) : false;
	if(false == IsTransferringBetweenMaps && false == L_HasMatchingGameplayTag)
	{
		const FQuat L_Quat(FRotator(0,GetControlRotation().Yaw, 0));
		const FVector R_RightVector = L_Quat.GetRightVector();
		AddMovementInput(R_RightVector, NewValue);
	}
}

void AZSCharacterWithAbilities::Dodge()
{
	if(GetOWSMovementComponent())
	{
		GetOWSMovementComponent()->DoDodge();
		USoundBase * L_Acceleration = Cast<USoundBase>(SoundBaseAcceleration.LoadSynchronous());
		checkf(nullptr != L_Acceleration, TEXT("The L_Acceleration is nullptr., Pleas Set Acceleration Sound."));
		if(L_Acceleration)
		{
			UGameplayStatics::PlaySoundAtLocation(this, L_Acceleration, GetActorLocation());
		}
	}
}

void AZSCharacterWithAbilities::ShowPlayersOnline()
{
	Server_ShowPlayersOnline();
}

void AZSCharacterWithAbilities::Server_ShowPlayersOnline_Implementation()
{
	AOWSGameMode * L_GameMode = GetGameMode();
	if(L_GameMode)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Characters Online:"));
		for(const FCharactersOnlineStruct & Iter :  L_GameMode->CharactersOnline)
		{
			const FString L_Log = FString::Printf(TEXT("(Character Name %s ) : (Zone Name %s )"), *Iter.CharName, *Iter.ZoneName);
		}
	}
}

bool AZSCharacterWithAbilities::Server_ShowPlayersOnline_Validate()
{
	return true;
}



