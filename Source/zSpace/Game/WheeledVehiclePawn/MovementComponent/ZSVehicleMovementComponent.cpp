// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AZSWheeledVehiclePawn* UZSVehicleMovementComponent::GetVehiclePawn() const
{
	AZSWheeledVehiclePawn * Pawn = Cast<AZSWheeledVehiclePawn>(GetOwner());
	return Pawn;
}

int32 UZSVehicleMovementComponent::GetVehicleSpeed()
{
	const AZSWheeledVehiclePawn * Pawn = GetVehiclePawn();
	int32 R_Speed = 0;
	if(IsValid(Pawn))
	{
		const float L_VectorLenght = Pawn->GetVelocity().Size();
		const float L_SpeedFloat = L_VectorLenght * 0.036f;
		R_Speed = UKismetMathLibrary::Abs_Int(L_SpeedFloat);
			
	}
	return R_Speed;
}

void UZSVehicleMovementComponent::SetIdleBrakeInput(const float& NewInput)
{
	IdleBrakeInput = NewInput;
}
