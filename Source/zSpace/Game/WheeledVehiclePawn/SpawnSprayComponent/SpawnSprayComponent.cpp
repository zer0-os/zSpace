// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/SpawnSprayComponent/SpawnSprayComponent.h"
#include "AIController.h"
#include "Game/WheeledVehiclePawn/ZSWheeledVehiclePawn.h"
#include "Game/WheeledVehiclePawn/DataAsset/TrackDataAsset/TrackDataAsset.h"
#include "Game/WheeledVehiclePawn/MovementComponent/ZSVehicleMovementComponent.h"
#include "Game/WheeledVehiclePawn/SurfaceTypesDetectActorComponent/SurfaceTypesDetectActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
USpawnSprayComponent::USpawnSprayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnSprayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(ROLE_Authority > GetOwnerRole() || UKismetSystemLibrary::IsStandalone(this))
	{
		BindSurfaceDetection();
	}
	
}


// Called every frame
void USpawnSprayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	SetParticleLocation();
}

bool USpawnSprayComponent::IsSetParticleLocation()
{
	const AZSWheeledVehiclePawn * Pawn = GetVehiclePawn();
	FVector L_OutDir(0);
	float L_OutLength = 0;
	if(IsValid(Pawn))
	{
		Pawn->GetVelocity().ToDirectionAndLength(L_OutDir, L_OutLength); 
	}
	if( ROLE_Authority > GetOwnerRole()
		|| (UKismetSystemLibrary::IsStandalone(this)
			&& L_OutLength  > 1)) // check vehicle is moving ?
				{
		return true;	
				}
	return false;	
}

void USpawnSprayComponent::SetParticleLocation(bool bForceSet)
{
	const bool bIsSetParticleLocation = IsSetParticleLocation();
	if(bIsSetParticleLocation || bForceSet)
	{
		const UZSVehicleMovementComponent * Movement = GetVehicleMovementComponent();
		if(IsValid(Movement))
		{
			for(int32 I = 0; I < Movement->WheelSetups.Num(); I++)
			{
				const FVector Location  = GetWheelLocation(I);
				if(SpawnedSpraies.IsValidIndex(I) && IsValid(SpawnedSpraies[I]))
				{
					SpawnedSpraies[I]->SetWorldLocation(Location);
					//UKismetSystemLibrary::DrawDebugSphere(this, Location, 50, 12, FLinearColor::Red, 0);
					//UE_LOG(LogTemp, Warning, TEXT("***************  bWasDeactivated = %d"), SpawnedTrackes[I]->bWasDeactivated);
					//UE_LOG(LogTemp, Warning, TEXT("***************  bWasActivate = %d"), SpawnedTrackes[I]->bWasActive);
					//UE_LOG(LogTemp, Warning, TEXT("***************  WasCompleted = %d"), SpawnedTrackes[I]->bWasCompleted);
				}
			}
    	}
		
	}
}

FVector USpawnSprayComponent::GetWheelLocation(int32 NewIndex)
{
	FVector R_WheelLocation(0);
	UZSVehicleMovementComponent * Movement = GetVehicleMovementComponent();
	if(IsValid(Movement))
	{
		AZSWheeledVehiclePawn * Pawn = GetVehiclePawn();
		if(Movement->WheelSetups.IsValidIndex(NewIndex))
		{
			const FName BoneName = Movement->WheelSetups[NewIndex].BoneName;
			R_WheelLocation  = Pawn->GetMesh()->GetSocketLocation(BoneName);
			if(Movement->Wheels.IsValidIndex(NewIndex) && Movement->Wheels[NewIndex] )
			{
				R_WheelLocation.Z -= Movement->Wheels[NewIndex]->WheelRadius;
			}
		}
	}
	return R_WheelLocation;
}

AZSWheeledVehiclePawn * USpawnSprayComponent::GetVehiclePawn()
{
	return 	Cast<AZSWheeledVehiclePawn>(GetOwner());
}

void USpawnSprayComponent::BindSurfaceDetection()
{
	AZSWheeledVehiclePawn * Pawn = GetVehiclePawn();
	if(IsValid(Pawn))
	{
		USurfaceTypesDetectActorComponent * L_SurfaceTypesDetectActorComponent = Cast<USurfaceTypesDetectActorComponent>(Pawn->GetComponentByClass(USurfaceTypesDetectActorComponent::StaticClass()));
		if(IsValid(L_SurfaceTypesDetectActorComponent))
		{
			L_SurfaceTypesDetectActorComponent->OnClientSurfaceTypeChange.AddUniqueDynamic(this, &USpawnSprayComponent::ClientSurfaceTypeChange);
		}
	}
}


void USpawnSprayComponent::ClientSurfaceTypeChange(UPhysicalMaterial * NewPhysicalMaterial, int32 I)
{
	if(IsValid(NewPhysicalMaterial))
	{
		TEnumAsByte<EPhysicalSurface> L_Surface = NewPhysicalMaterial->SurfaceType;
		//UKismetSystemLibrary::PrintString(this,TEXT("Surfas type") + GetNameSafe(NewPhysicalMaterial));
		if (IsValid(TrackDataAsset))
		{	
			UParticleSystem * L_Particle = TrackDataAsset->GetSprayParticle(L_Surface);
			FRotator L_Rotation(0);
			TrackDataAsset->GetRotation(L_Surface,L_Rotation);
			const FVector L_Location = GetWheelLocation(I);
			SpawnSpray(L_Particle, L_Location, NAME_None, I, L_Rotation);
		}
	}
	else
	{
		SetTemplate(nullptr, I);
	}
	
}

UZSVehicleMovementComponent* USpawnSprayComponent::GetVehicleMovementComponent()
{
	UZSVehicleMovementComponent * R_Movement = nullptr;
	AZSWheeledVehiclePawn * Vehicle = GetVehiclePawn();
	if(IsValid(Vehicle))
	{
		R_Movement = Cast<UZSVehicleMovementComponent>(Vehicle->GetVehicleMovementComponent());	
	}
	return R_Movement;
}


void USpawnSprayComponent::SetTemplate(UParticleSystem * NewParticle, int32 NewIndex)
{
	if(SpawnedSpraies.IsValidIndex(NewIndex))
	{
		UParticleSystemComponent *  L_SpawnParticle = SpawnedSpraies[NewIndex];
		if(IsValid(L_SpawnParticle))
		{
			L_SpawnParticle->SetTemplate(NewParticle);
			L_SpawnParticle->ActivateSystem(true);
			UE_LOG(LogTemp, Warning, TEXT("****************  %s ***********"), NewParticle == nullptr ? TEXT("nullptr") : TEXT("Not nullptr"));
			//L_SpawnParticle->ActivateSystem();
		}
	}
	
}

 void USpawnSprayComponent::SpawnSpray(UParticleSystem * NewParticle, FVector Location, FName SocketName, int32 NewIndex, const FRotator& NewRotation)
{
	//UKismetSystemLibrary::PrintString(this,"************************************");
	const AZSWheeledVehiclePawn * Vehicle = GetVehiclePawn();
	USkeletalMeshComponent * L_Mesh = Vehicle->GetMesh();
	if(!SpawnedSpraies.IsValidIndex(NewIndex))
	{
		UParticleSystemComponent *  L_SpawnParticle = NewObject<UParticleSystemComponent>(this);
		if(IsValid(L_SpawnParticle))
		{
			SpawnedSpraies.Add(L_SpawnParticle);
			SetParticleLocation(true);
			L_SpawnParticle->SetWorldRotation(NewRotation);
			L_SpawnParticle->SetTemplate(NewParticle);
			L_SpawnParticle->bAutoActivate = true;
			L_SpawnParticle->bAutoDestroy = true;
			L_SpawnParticle->ActivateSystem(false);
			L_SpawnParticle->RegisterComponentWithWorld(GetWorld());
		}
	}
	else
	{
		SetTemplate(NewParticle, NewIndex);
	}
}
