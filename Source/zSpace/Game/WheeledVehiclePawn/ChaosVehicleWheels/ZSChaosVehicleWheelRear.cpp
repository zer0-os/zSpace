// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/ChaosVehicleWheels/ZSChaosVehicleWheelRear.h"

UZSChaosVehicleWheelRear::UZSChaosVehicleWheelRear()
{
	WheelRadius = 18.f;
	WheelWidth = 20.0f;
	//FrictionForceMultiplier = 2.0f;
	//CorneringStiffness = 500.0f;
	bAffectedByEngine = false;
	bAffectedByHandbrake = false;
	bAffectedBySteering = true;
	AxleType = EAxleType::Front;
	SpringRate = 200.0f;
	SpringPreload = 100.f;
	SuspensionDampingRatio = 0.5f;
	WheelLoadRatio = 0.0f;
	RollbarScaling = 0.5f;
	SuspensionMaxRaise = 8;
	SuspensionMaxDrop = 12;
	WheelLoadRatio = 0.5f;
	MaxSteerAngle = 40.f;
}
