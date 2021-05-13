// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/AnimNotify/AnimNotify_PutLeftFoot.h"

#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimInstance.h"
#include "zSpace/ZSCharacterWithAbilities/Components/DetectSurfaceTypeComponent/DetectSurfaceTypeComponent.h"

void UAnimNotify_PutLeftFoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		UZSAnimInstance* AI = Cast<UZSAnimInstance>(MeshComp->GetAnimInstance());
		if (IsValid(AI))
		{
			AI->SetCharacterFood(ECharacterFootType::LEFT);		
		}
	}
}
