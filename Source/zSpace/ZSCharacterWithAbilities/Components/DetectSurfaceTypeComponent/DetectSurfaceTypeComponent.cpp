// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystem.h"
#include "zSpace/ZSCharacterWithAbilities/Components/CharacterMovementComponent/ZSCharacterMovementComponent.h"

// Sets default values for this component's properties
UDetectSurfaceTypeComponent::UDetectSurfaceTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
		
	//EObjectTypeQuery> >  ObjectTypes
	ObjectTypes.Add( UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	ObjectTypes.Add( UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ACharacter * L_Character = Cast<ACharacter>(GetOwner());
	if(L_Character)
	{
		UZSCharacterMovementComponent * MovementComponent = Cast<UZSCharacterMovementComponent>(L_Character->GetCharacterMovement());
		if(MovementComponent)
		{
			MovementComponent->OnMovementModeChangedDelegate.AddDynamic(this, &UDetectSurfaceTypeComponent::OnMovementModeChanged);
		}
	}
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


FRotator UDetectSurfaceTypeComponent::GetFootRotationByCharacterFootType(ECharacterFootType NewCharacterFootType)
{
	USkeletalMeshComponent * L_SkeletalMeshComponent = GetSkeletalMesh();
	FRotator R_Rotation = FRotator::ZeroRotator;

	if(L_SkeletalMeshComponent)
	{
		if(ECharacterFootType::LEFT == NewCharacterFootType)
		{
			R_Rotation = L_SkeletalMeshComponent->GetSocketRotation(FName(LeftFootBoneName));
		}
		else if(ECharacterFootType::RIGHT == NewCharacterFootType)
		{
			R_Rotation = L_SkeletalMeshComponent->GetSocketRotation(FName(RightFootBoneName));
		}
	}
	return R_Rotation;
}

void UDetectSurfaceTypeComponent::PutFootOnGround(ECharacterFootType NewCharacterFootType)
{
	// const FString L_CharacterFootType = UEnum::GetValueAsString(NewCharacterFootType);
	//UE_LOG(LogTemp, Log, TEXT("Foot = %s"), *L_CharacterFootType);
	const FVector L_Start = GetFootLocationByCharacterFootType(NewCharacterFootType);
	const FVector L_End = (FVector::UpVector * - 250) + L_Start;
	TArray<AActor *> L_ActorsToIgnore;
	TArray<FHitResult> L_OutHits;

	FRotator L_Rotation = GetFootRotationByCharacterFootType(NewCharacterFootType);
	L_Rotation.Yaw += -90;

	const bool bIsBlocking = UKismetSystemLibrary::LineTraceMultiForObjects(GetOwner(), L_Start, L_End, ObjectTypes, true, L_ActorsToIgnore, EDrawDebugTrace::None, L_OutHits, true);
	if(bIsBlocking)
	{
		for(const FHitResult & IterHitResult : L_OutHits)
		{
			if(IterHitResult.bBlockingHit && IterHitResult.PhysMaterial.IsValid() )
			{
				UPhysicalMaterial * L_PhysicalMaterial = IterHitResult.PhysMaterial.Get();
				//UE_LOG(LogTemp, Log, TEXT("CompoentName =  %s ,  Actor Name %s ***************** Physics Material name = %s"), *IterHitResult.Component->GetName(), *IterHitResult.Actor->GetName(), *L_PhysicalMaterial->GetName());
				if(CharacterUnderFootSurfaceDA)
				{
					bool bIsValid = false;
					const FCharacterUnderFootSurfaceData L_CharacterUnderFootSurfaceData =  CharacterUnderFootSurfaceDA->GetCharacterUnderFootSurfaceDataByPhysicsType(L_PhysicalMaterial, bIsValid);
					if(bIsValid)
					{
						PlayRandomSound(L_CharacterUnderFootSurfaceData, IterHitResult.Location);
						SpawnParticle(L_CharacterUnderFootSurfaceData, IterHitResult.Location);
						SpawnFootStepDecal(L_CharacterUnderFootSurfaceData, IterHitResult.Location, L_Rotation);
					}
					return;
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

void UDetectSurfaceTypeComponent::SpawnFootStepDecal(const FCharacterUnderFootSurfaceData& NewCharacterUnderFootSurfaceData, const FVector& NewLocation, const FRotator& NewRotation)
{
	UMaterialInstance* L_FootStepMaterial = NewCharacterUnderFootSurfaceData.SurfaceFootStepMaterial.LoadSynchronous();
	if (L_FootStepMaterial)
	{
		UKismetSystemLibrary::PrintString(GetOwner(), L_FootStepMaterial->GetName());
		FVector Size = FVector(10, 15, 20);
		UGameplayStatics::SpawnDecalAtLocation(GetOwner(), L_FootStepMaterial, Size, NewLocation, NewRotation, 10);
	}
}

void UDetectSurfaceTypeComponent::OnMovementModeChanged(EMovementMode NewPreviousMovementMode, EMovementMode NewCurrentMovementMode, uint8 NewPreviousCustomMode)
{
	//UE_LOG(LogTemp, Warning, TEXT("NewPreviousMovementMode = %s,   NewCurrentMovementMode = %s"), *UEnum::GetValueAsString<EMovementMode>(NewPreviousMovementMode), *UEnum::GetValueAsString<EMovementMode>(NewCurrentMovementMode));
	checkf(nullptr != CharacterUnderFootSurfaceDA, TEXT("The CharacterUnderFootSurfaceDA is nulltrp. Please set data asset."));
	if(IsValid(CharacterUnderFootSurfaceDA))
	{
		const FVector L_Start = GetFootLocationByCharacterFootType(ECharacterFootType::LEFT);
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
					bool L_IsStatus = false;
					const FFootHitGroundData L_FootHitGroundData =  CharacterUnderFootSurfaceDA->GetFootHitGroundDataByMovementMode(NewPreviousMovementMode, NewCurrentMovementMode, L_PhysicalMaterial, L_IsStatus);
					if(L_IsStatus)
					{
						USoundBase * L_SoundBase = 	L_FootHitGroundData.SoundBase.LoadSynchronous();
						checkf(nullptr != L_SoundBase, TEXT("The L_SoundBase is nulltrp. Please "));
						if(IsValid(L_SoundBase))
						{
							UGameplayStatics::PlaySoundAtLocation(GetOwner(), L_SoundBase, L_Start);
						}
					}
					return;
				}
			}
		}
	}
}
