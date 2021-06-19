// Copyright 2020 Sabre Dart Studios


#include "BallisticLineComponent.h"

#include <activation.h>
#include <fstream>

#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBallisticLineComponent::UBallisticLineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickInterval = 0.1;
	DecalSize = FVector(10, 100, 100);
	// ...
}


// Called when the game starts
void UBallisticLineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBallisticLineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateSplinePoints();
	UpdateSplineMeshPosition();
	// ...
}

void UBallisticLineComponent::SpawnDecal()
{
	DecalComponentDestination = UGameplayStatics::SpawnDecalAtLocation(this, DecalMaterialDestination, DecalSize, TeleportLocation);
}

void UBallisticLineComponent::CreateSplineComponent()
{
	AActor * OwnerActor = GetOwner();
	if(IsValid(OwnerActor) && nullptr == SplineComponentBallisticLine)
	{
		SplineComponentBallisticLine = NewObject<USplineComponent>(OwnerActor);
		checkf(nullptr != SplineComponentBallisticLine, TEXT("The SplineComponentBallisticLine is nullptr."));
		SplineComponentBallisticLine->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		SplineComponentBallisticLine->Duration = SplinePointAndSplineMeshCount;
		SplineComponentBallisticLine->RegisterComponent();
		UE_LOG(LogTemp, Log, TEXT("ZSpace: Created SplineComponent"));
	}
}

void UBallisticLineComponent::ClearSplineMeshComponent()
{
	for(USplineMeshComponent * Iter : SplineMeshComponentArray)
	{
		if(IsValid(Iter))
		{
			Iter->DestroyComponent();	
		}
	}
	SplineMeshComponentArray.Empty();
	bIsCreatedSplineMeshComponent = false;
}

void UBallisticLineComponent::CreateSplineMeshComponents()
{
	if(SplineMeshComponentArray.Num() == 0)
	{
		bIsCreatedSplineMeshComponent = true;
		AActor * OwnerActor = GetOwner();
		for(uint8 I = 0; I < SplinePointAndSplineMeshCount ; I++)
		{
			USplineMeshComponent * L_SplineMeshComponent = NewObject<USplineMeshComponent>(OwnerActor);
			checkf(nullptr != L_SplineMeshComponent, TEXT("The SplineMeshComponent is nullptr."))
			L_SplineMeshComponent->RegisterComponent();
			L_SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
			L_SplineMeshComponent->Mobility = EComponentMobility::Movable;
			L_SplineMeshComponent->SetStartScale(FVector2D(0.1, 0.1));
			L_SplineMeshComponent->SetEndScale(FVector2D(0.1, 0.1));
			L_SplineMeshComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			L_SplineMeshComponent->SetStaticMesh(StaticMeshBallisticLine);
			L_SplineMeshComponent->SetMaterial(0, MaterialInterfaceBallisticLine);
			SplineMeshComponentArray.Add(L_SplineMeshComponent);
		}
	}
}

void UBallisticLineComponent::UpdateSplinePoints()
{
	if(IsValid(SplineComponentBallisticLine) && true == bIsCreatedSplineMeshComponent)
	{
		FPredictProjectilePathParams PredictParams;
		PredictParams.bTraceComplex = false;
		PredictParams.StartLocation = GetComponentLocation();
		PredictParams.LaunchVelocity =  GetAttachParent()->GetForwardVector() * 600;
		PredictParams.DrawDebugTime = 2;
		PredictParams.DrawDebugType = EDrawDebugTrace::None;
		PredictParams.SimFrequency = 15;
		PredictParams.MaxSimTime = 2;
		PredictParams.OverrideGravityZ = 0;
		PredictParams.bTraceComplex = false;
		PredictParams.bTraceWithCollision = true;
		PredictParams.ActorsToIgnore.Add(GetOwner());
		//PredictParams.ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		//PredictParams.ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		FPredictProjectilePathResult PredictResult;
		bool bIsPredictSuccess = UGameplayStatics::PredictProjectilePath(GetOwner(), PredictParams, PredictResult );
		if(bIsPredictSuccess)
		{
			TArray<FPredictProjectilePathPointData> PathData = PredictResult.PathData;
			UE_LOG(LogTemp, Log, TEXT("*** Count %d predict, IsPredictSucess = %d "), PredictResult.PathData.Num(), bIsPredictSuccess);
			TArray<FVector > Points;
			for(FPredictProjectilePathPointData Iter : PathData)
			{
				Points.Add(Iter.Location);
			}
			SplineComponentBallisticLine->SetSplinePoints(Points, ESplineCoordinateSpace::World, true);
			bIsTeleport = true;
			TeleportLocation = PredictResult.HitResult.Location;
			if(IsValid(DecalComponentDestination))
			{
				FRotator L_Rotation = UKismetMathLibrary::MakeRotationFromAxes(PredictResult.HitResult.ImpactNormal, FVector(0), FVector(0));
				DecalComponentDestination->SetWorldLocationAndRotation(TeleportLocation, L_Rotation);
			}
		}
	}
	
}

void UBallisticLineComponent::UpdateSplineMeshPosition()
{
	if(IsValid(SplineComponentBallisticLine) && true == bIsCreatedSplineMeshComponent)
	{
		//   
		for(uint8 Index0 = 0; Index0 < SplinePointAndSplineMeshCount ; Index0++)
		{
			const uint8 Index1 = Index0 + 1;
			if(SplineMeshComponentArray.IsValidIndex(Index0))
			{
				USplineMeshComponent * L_SplineMeshComponent = SplineMeshComponentArray[Index0];
				checkf(nullptr != L_SplineMeshComponent, TEXT("The L_SplineMeshComponent is nullptr."));
				const FVector StartPosition =  SplineComponentBallisticLine->GetLocationAtTime(Index0, ESplineCoordinateSpace::World);
				const FVector StartTangent =  SplineComponentBallisticLine->GetTangentAtTime(Index0, ESplineCoordinateSpace::World);
				const FVector EndPosition =  SplineComponentBallisticLine->GetLocationAtTime(Index1, ESplineCoordinateSpace::World);
				const FVector EndTangent =  SplineComponentBallisticLine->GetTangentAtTime(Index1, ESplineCoordinateSpace::World);
				L_SplineMeshComponent->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent, true);
			}
		}
	}
}

void UBallisticLineComponent::ShowBallisticLine()
{
	bIsTeleport = false;
	CreateSplineComponent();
	CreateSplineMeshComponents();
	SpawnDecal();	
	
}

void UBallisticLineComponent::HideBallisticLine()
{
	ClearSplineMeshComponent();
	AActor * L_Actor = GetOwner();
	if(IsValid(L_Actor) && bIsTeleport)
	{
		TeleportLocation.Z += 90;
		L_Actor->TeleportTo(TeleportLocation, FRotator(), true);
	}
	if(IsValid(DecalComponentDestination))
	{
		DecalComponentDestination->DestroyComponent();	
	}
}

