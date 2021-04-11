// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDetectSurfaceTypeComponent::UDetectSurfaceTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
		}
		else if(ECharacterFootType::RIGHT == NewCharacterFootType)
		{
			R_Location = L_SkeletalMeshComponent->GetSocketLocation(FName(RightFootBoneName));
		}
	}
	return R_Location;
}


void UDetectSurfaceTypeComponent::PutFootOnGround(ECharacterFootType NewCharacterFootType)
{
	// const FString L_CharacterFootType = UEnum::GetValueAsString(NewCharacterFootType);
	//UE_LOG(LogTemp, Log, TEXT("Foot = %s"), *L_CharacterFootType);
	const FVector L_Start = GetFootLocationByCharacterFootType(NewCharacterFootType);
	const FVector L_End = (FVector::UpVector * - 100) + L_Start;
	TArray<AActor *> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	const bool bIsBlocking = UKismetSystemLibrary::LineTraceMulti(GetOwner(), L_Start, L_End, ETraceTypeQuery::TraceTypeQuery2, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);
	if(bIsBlocking)
	{
		for(const FHitResult & IterHitResult : OutHits)
		{
			if(IterHitResult.bBlockingHit && IterHitResult.PhysMaterial.IsValid() )
			{
				UPhysicalMaterial * L_PhysicalMaterial = IterHitResult.PhysMaterial.Get();
				if(CharacterUnderFootSurfaceDA)
				{
					bool bIsValid = false;
					const FCharacterUnderFootSurfaceData L_CharacterUnderFootSurfaceData =  CharacterUnderFootSurfaceDA->GetCharacterUnderFootSurfaceDataByPhysicsType(L_PhysicalMaterial, bIsValid);
					if(bIsValid)
					{
						PlayRandomSound(L_CharacterUnderFootSurfaceData, L_Start);
					}
				}
			}
		}
	}
}

void UDetectSurfaceTypeComponent::PlayRandomSound(FCharacterUnderFootSurfaceData NewCharacterUnderFootSurfaceData, const FVector & NewLocation)
{
	const int32 L_Num = NewCharacterUnderFootSurfaceData.SurfaceSoundBaseArray.Num();
	const int32 L_Index = UKismetMathLibrary::RandomInteger(L_Num);
	USoundBase * L_Sound = NewCharacterUnderFootSurfaceData.SurfaceSoundBaseArray[L_Index];
	//UE_LOG(LogTemp, Log, TEXT(" Num = %d  Index = %d ----"), L_Num, L_Index);
	UGameplayStatics::PlaySoundAtLocation(GetOwner(), L_Sound, NewLocation);
}
