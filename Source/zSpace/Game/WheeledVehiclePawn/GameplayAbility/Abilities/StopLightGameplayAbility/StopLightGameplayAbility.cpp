// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WheeledVehiclePawn/GameplayAbility/Abilities/StopLightGameplayAbility/StopLightGameplayAbility.h"

#include "WheeledVehiclePawn.h"
#include "Net/UnrealNetwork.h"

FName UStopLightGameplayAbility::VehicleStopLightParamName = "EmissiveColorStopLights";

UStopLightGameplayAbility::UStopLightGameplayAbility()
{
	
}

USkeletalMeshComponent* UStopLightGameplayAbility::GetVehicleSkeletalMeshComponent()
{
	AWheeledVehiclePawn * VehiclePawn = Cast<AWheeledVehiclePawn>(GetOwningActorFromActorInfo());
	if(IsValid(VehiclePawn))
	{
		UE_LOG(LogTemp, Log, TEXT("****************** Vehicle Pawn Is Valid *************************"));
		return VehiclePawn->GetMesh();	
	}
	UE_LOG(LogTemp, Log, TEXT("****************** Vehicle Pawn Is not  Valid *************************"));
	return nullptr;
}

void UStopLightGameplayAbility::OnRepIsStopLight()
{
	USkeletalMeshComponent * Mesh = GetVehicleSkeletalMeshComponent();
	if(IsValid(Mesh) && Mesh->GetOwnerRole() != ROLE_Authority )
	{
		const int32 Num = Mesh->GetNumMaterials();
		for(int32 I = 0; I < Num; I++)
		{
			UMaterialInstanceDynamic * MatDyn = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(I));
			if(nullptr == MatDyn)
			{
				MatDyn = Mesh->CreateAndSetMaterialInstanceDynamic(I);
			}
			if(MatDyn)
			{
				if(bIsStopLight)
				{
					MatDyn->SetScalarParameterValue(VehicleStopLightParamName, StopEmissiveColorStopLightsMaxValue);
				}
				else
				{
					MatDyn->SetScalarParameterValue(VehicleStopLightParamName, 0);
				}
			}
		}
	}
}

void UStopLightGameplayAbility::SetIsStopLight(bool NewIsStopLight)
{
	bIsStopLight = NewIsStopLight;
}

void UStopLightGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStopLightGameplayAbility, bIsStopLight);
}
