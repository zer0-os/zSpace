// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
UDetectSurfaceTypeComponent::UDetectSurfaceTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
		
	//EObjectTypeQuery> >  ObjectTypes
	ObjectTypes.Add( UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	ObjectTypes.Add( UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	
	// ...
}


// Called when the game starts
void UDetectSurfaceTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDetectSurfaceTypeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

USkeletalMeshComponent* UDetectSurfaceTypeComponent::GetSkeletalMesh() const
{
	USkeletalMeshComponent * R_SkeletalMeshComponent = nullptr;
	ACharacter * L_Character = Cast<ACharacter>(GetOwner());
	if(L_Character)
	{
		R_SkeletalMeshComponent =  L_Character->GetMesh();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: The DetectSurfaceTypeComponent isn't child of Character class"));
	}
	return R_SkeletalMeshComponent;
}

FVector UDetectSurfaceTypeComponent::GetFootLocationByCharacterFootType(ECharacterFootType NewCharacterFootType)
{
	USkeletalMeshComponent * L_SkeletalMeshComponent = GetSkeletalMesh();
	FVector R_Location = FVector::ZeroVector;
	if(L_SkeletalMeshComponent)
	{
		if(ECharacterFootType::LEFT == NewCharacterFootType)
		{
			R_Location = L_SkeletalMeshComponent->GetSocketLocation(FName(LeftFootBoneName));
			R_Location.Z += 150;
		}
		else if(ECharacterFootType::RIGHT == NewCharacterFootType)
		{
			R_Location = L_SkeletalMeshComponent->GetSocketLocation(FName(RightFootBoneName));
			R_Location.Z += 150;
		}
	}
	return R_Location;
}


void UDetectSurfaceTypeComponent::PutFootOnGround(ECharacterFootType NewCharacterFootType)
{
	// const FString L_CharacterFootType = UEnum::GetValueAsString(NewCharacterFootType);
	//UE_LOG(LogTemp, Log, TEXT("Foot = %s"), *L_CharacterFootType);
	const FVector L_Start = GetFootLocationByCharacterFootType(NewCharacterFootType);
	const FVector L_End = (FVector::UpVector * - 250) + L_Start;
	TArray<AActor *> L_ActorsToIgnore;
	TArray<FHitResult> L_OutHits;
	const bool bIsBlocking = UKismetSystemLibrary::LineTraceMultiForObjects(GetOwner(), L_Start, L_End, ObjectTypes, true, L_ActorsToIgnore, EDrawDebugTrace::None, L_OutHits, true);
	if(bIsBlocking)
	{
		for(const FHitResult & IterHitResult : L_OutHits)
		{
			if(IterHitResult.bBlockingHit && IterHitResult.PhysMaterial.IsValid() )
			{
				UPhysicalMaterial * L_PhysicalMaterial = IterHitResult.PhysMaterial.Get();
				//UE_LOG(LogTemp, Log, TEXT(" %s ***************** Physics Material name = %s"), *IterHitResult.Actor->GetName(), *L_PhysicalMaterial->GetName());
				if(CharacterUnderFootSurfaceDA)
				{
					bool bIsValid = false;
					const FCharacterUnderFootSurfaceData L_CharacterUnderFootSurfaceData =  CharacterUnderFootSurfaceDA->GetCharacterUnderFootSurfaceDataByPhysicsType(L_PhysicalMaterial, bIsValid);
					if(bIsValid)
					{
						PlayRandomSound(L_CharacterUnderFootSurfaceData, IterHitResult.Location);
						SpawnParticle(L_CharacterUnderFootSurfaceData, IterHitResult.Location);
						return;
					}
				}
			}
		}
	}
}

void UDetectSurfaceTypeComponent::PlayRandomSound(const FCharacterUnderFootSurfaceData & NewCharacterUnderFootSurfaceData, const FVector & NewLocation)
{
	const int32 L_Num = NewCharacterUnderFootSurfaceData.SurfaceSoundBaseArray.Num();
	const int32 L_Index = UKismetMathLibrary::RandomInteger(L_Num);
	USoundBase * L_Sound = NewCharacterUnderFootSurfaceData.SurfaceSoundBaseArray[L_Index].LoadSynchronous();
	//UE_LOG(LogTemp, Log, TEXT(" Num = %d  Index = %d ----"), L_Num, L_Index);
	if(L_Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetOwner(), L_Sound, NewLocation);
	}
}

void UDetectSurfaceTypeComponent::SpawnParticle(const FCharacterUnderFootSurfaceData & NewCharacterUnderFootSurfaceData, const FVector& NewLocation)
{
	UParticleSystem * L_ParticleSystem = NewCharacterUnderFootSurfaceData.SurfaceParticleSystem.LoadSynchronous();
	if(L_ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), L_ParticleSystem, NewLocation);
	}
}
