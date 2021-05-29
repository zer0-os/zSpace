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

UAnimMontage* UAnimMontageLocomotionDataAsset::GetAnimMontageByGaitAndDirection(const EPlayerGait& PlayerGait,
	const EPlayerMoveDirection PlayerMoveDirection)
{
	for (const auto& Montage : AnimMontageLocomotionArray)
	{
		if (IsValid(Montage.AnimMontage))
		{
			if (Montage.PlayerGait == PlayerGait && Montage.PlayerMoveDirection == PlayerMoveDirection)
			{
				return Montage.AnimMontage;
			}
		}
	}
		
	return nullptr;
}

bool UAnimMontageLocomotionDataAsset::HasThisMontage(UAnimMontage* ThisMontage)
{
	for (const auto& Montage : AnimMontageLocomotionArray)
	{
		if (IsValid(Montage.AnimMontage) && Montage.AnimMontage == ThisMontage)
		{
			return true;
		}
	}
		
	return false;
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
