// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/CharacterMeshesDataAsset.h"
#include "Engine/SkeletalMesh.h"

USkeletalMesh* UCharacterMeshesDataAsset::GetMeshByName(const FName& Name) const
{
	for(const FCharacterMeshData & Iter :  CharacterMeshDataArray)
	{
		const FString SkeletalMeshNameParam = Name.ToString();
		const FString SkeletalMeshName = Iter.CharacterMesh->GetName();
		if(IsValid(Iter.CharacterMesh) && SkeletalMeshName == SkeletalMeshNameParam)
		{
			return Iter.CharacterMesh;
		}
	}
	return nullptr;
}

FName UCharacterMeshesDataAsset::GetNameByMesh(const USkeletalMesh* Mesh) const
{
	for(const FCharacterMeshData & Iter :  CharacterMeshDataArray)
	{
		if(IsValid(Mesh) && Iter.CharacterMesh == Mesh)
		{
			return FName(Iter.CharacterMesh->GetName());
		}
	}
	return NAME_None;
}

TArray<USkeletalMesh*> UCharacterMeshesDataAsset::GetAllSkeletalMeshes()
{
	TArray<USkeletalMesh *> R_SkeletalMeshes;
	for(const FCharacterMeshData & Iter :  CharacterMeshDataArray)
	{
		R_SkeletalMeshes.Add(Iter.CharacterMesh);
	}
	return R_SkeletalMeshes;	
}


void UCharacterMeshesDataAsset::GetAllCharacterNames(TArray<FName> & CharacterNames)
{
	for (const FCharacterMeshData & Iter : CharacterMeshDataArray)
	{
		CharacterNames.Add(FName(Iter.CharacterMesh->GetName()));
	}
}

void UCharacterMeshesDataAsset::GetDefaultSkeletalMeshName(FString& NewDefaultSkeletalMeshName)
{
	for (const FCharacterMeshData & Iter : CharacterMeshDataArray)
	{
		if(Iter.bIsDefaultCharacter && IsValid(Iter.CharacterMesh))
		{
			NewDefaultSkeletalMeshName = Iter.CharacterMesh->GetName();
			return;
		}
	}
	NewDefaultSkeletalMeshName = "";
}


#if WITH_EDITOR
void UCharacterMeshesDataAsset::PreEditChange(FEditPropertyChain& PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
	for(FCharacterMeshData & Iter : CharacterMeshDataArray)
	{
		Iter.bIsDefaultCharacter = false;
	}
}
#endif

