// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/AnimInstances/ZSAnimationTypes.h"

#include "zSpace/zSpace.h"

UAnimMontage* UAnimMontageLocomotionDataAsset::GetAnimMontageByGaitAndFoot(const EPlayerGait& PlayerGait,
                                                                           const ECharacterFootType& CharacterFoot)
{
	for (const auto& Montage : AnimMontageLocomotionArray)
	{
		if (IsValid(Montage.AnimMontage))
		{
			if (Montage.PlayerGait == PlayerGait && Montage.CharacterFoot == CharacterFoot)
			{
				return Montage.AnimMontage;	
			}
		}
	}
		
	return nullptr;
}

TArray<UAnimMontage*> UAnimMontageLocomotionDataAsset::GetAllMontages() const
{
	TArray<UAnimMontage*> Result;

	for (const auto& Montage : AnimMontageLocomotionArray)
	{
		Result.Add(Montage.AnimMontage);
	}

	return Result;
}
