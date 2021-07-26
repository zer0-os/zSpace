// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/SpringArmComponent/ZSSpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


float UZSSpringArmComponent::PlayerCameraManagerViewPitchMin = 0;
float UZSSpringArmComponent::PlayerCameraManagerViewPitchMax = 0; 
float UZSSpringArmComponent::PlayerCameraManagerViewRollMin = 0;	
float UZSSpringArmComponent::PlayerCameraManagerViewRollMax = 0;
float UZSSpringArmComponent::PlayerCameraManagerViewYawMin = 0;	
float UZSSpringArmComponent::PlayerCameraManagerViewYawMax = 0;

#if WITH_EDITOR
void UZSSpringArmComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : FName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UZSSpringArmComponent, bIsUseControllerInput))
	{
		if(bIsUseControllerInput)
		{
			bUsePawnControlRotation = false;
		}
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UZSSpringArmComponent, bUsePawnControlRotation))
	{
		if(bIsUseControllerInput)
		{
			bIsUseControllerInput = false;
		}
	}
}
#endif


void UZSSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateSpringArmRotation();
}

void UZSSpringArmComponent::UpdateSpringArmRotation()
{
	if(bIsUseControllerInput && IsActive())
	{
		FRotator DesiredRot = GetDesiredRotation();
		if(APawn * OwningPawn = Cast<APawn>(GetOwner()))
		{
			const FRotator PawnViewRotation = OwningPawn->GetViewRotation();
			if(DesiredRot != PawnViewRotation)
			{
				DesiredRot = PawnViewRotation;
			}
		}
		// If inheriting rotation, check options for which components to inherit
		if (!IsUsingAbsoluteRotation())
		{
			const FRotator LocalRelativeRotation = GetRelativeRotation();
			if (!bIsUseControllerInputPitch)
			{
				DesiredRot.Pitch = LocalRelativeRotation.Pitch;
			}
			if (!bIsUseControllerInputYaw)
			{
				DesiredRot.Yaw = LocalRelativeRotation.Yaw;
			}

			if (!bIsUseControllerInputRoll)
			{
				DesiredRot.Roll = LocalRelativeRotation.Roll;
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("****************** Rotation = %s ******************** "), *DesiredRot.ToString());
		SetRelativeRotation(DesiredRot);
	}
	
	
}

void UZSSpringArmComponent::SetLimitation(FRotator NewMinLimitRotation, FRotator NewMaxLimitRotation)
{
	APlayerCameraManager * PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if(IsValid(PlayerCameraManager))
	{
		PlayerCameraManagerViewPitchMin = PlayerCameraManager->ViewPitchMin;
		PlayerCameraManagerViewPitchMax = PlayerCameraManager->ViewPitchMax;
		
		PlayerCameraManagerViewRollMin = PlayerCameraManager->ViewRollMin;	
		PlayerCameraManagerViewRollMax = PlayerCameraManager->ViewRollMax;
		
		PlayerCameraManagerViewYawMin = PlayerCameraManager->ViewYawMin;	
		PlayerCameraManagerViewYawMax = PlayerCameraManager->ViewYawMax;
		
		
		PlayerCameraManager->ViewPitchMin = NewMinLimitRotation.Pitch;	
		PlayerCameraManager->ViewPitchMax = NewMaxLimitRotation.Pitch;
		
		PlayerCameraManager->ViewRollMin = NewMinLimitRotation.Roll;	
		PlayerCameraManager->ViewRollMax = NewMaxLimitRotation.Roll;
		
		PlayerCameraManager->ViewYawMin = NewMinLimitRotation.Yaw;	
		PlayerCameraManager->ViewYawMax = NewMaxLimitRotation.Yaw;
	}
	
}

void UZSSpringArmComponent::UpdateLimitation()
{
	if(bIsUseControllerInput)
	{
		SetLimitation(MinLimitRotation, MaxLimitRotation);
	}
}

void UZSSpringArmComponent::ResetPlayerCameraManagerRotationLimit()
{
	APlayerCameraManager * PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if(IsValid(PlayerCameraManager))
	{
		PlayerCameraManager->ViewPitchMin = PlayerCameraManagerViewPitchMin ;
		PlayerCameraManager->ViewPitchMax = PlayerCameraManagerViewPitchMax;
		
		PlayerCameraManager->ViewRollMin = PlayerCameraManagerViewRollMin; 
		PlayerCameraManager->ViewRollMax = PlayerCameraManagerViewRollMax;
		
		PlayerCameraManager->ViewYawMin = PlayerCameraManagerViewYawMin;	
		PlayerCameraManager->ViewYawMax = PlayerCameraManagerViewYawMax;
	}
}
