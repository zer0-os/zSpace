// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/ChaosVehicleWheels/ZSChaosVehicleWheelFront.h"

UZSChaosVehicleWheelFront::UZSChaosVehicleWheelFront()
{
	WheelRadius = 18.f;
	WheelWidth = 20.0f;
	//FrictionForceMultiplier = 2.0f;
	//CorneringStiffness = 500.0f;
	bAffectedByEngine = true;
	bAffectedByHandbrake = true;
	bAffectedBySteering = true;
	AxleType = EAxleType::Front;
	SpringRate = 200.0f;
	SpringPreload = 100.f;
	SuspensionDampingRatio = 0.5f;
	WheelLoadRatio = 0.0f;
	RollbarScaling = 0.5f;
	SuspensionMaxRaise = 8;
	SuspensionMaxDrop = 12.0f;
	WheelLoadRatio = 0.5f;
	SweepShape = ESweepShape::Spherecast;
}
