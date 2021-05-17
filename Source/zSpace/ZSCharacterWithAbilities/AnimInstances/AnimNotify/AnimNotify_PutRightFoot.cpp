// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/AnimNotify/AnimNotify_PutRightFoot.h"

#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimInstance.h"

void UAnimNotify_PutRightFoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		UZSAnimInstance* AI = Cast<UZSAnimInstance>(MeshComp->GetAnimInstance());
		if (IsValid(AI))
		{
			AI->SetCharacterFood(ECharacterFootType::RIGHT);
		}
	}
}
