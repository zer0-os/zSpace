// Copyright 2020 Sabre Dart Studios


#include "ZSCameraComponent.h"

ECameraPositionType UZSCameraComponent::GetCameraPositionType() const
{
	return CameraPositionType;
}

void UZSCameraComponent::SetCameraPositionType(ECameraPositionType NewCameraPositionType)
{
	CameraPositionType = NewCameraPositionType;
}
