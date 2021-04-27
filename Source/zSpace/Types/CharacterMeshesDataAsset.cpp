// Copyright 2020 Sabre Dart Studios


#include "zSpace/Types/CharacterMeshesDataAsset.h"

USkeletalMesh* UCharacterMeshesDataAsset::GetMeshByName(const FName& Name) const
{
	USkeletalMesh* const* Mesh = CharacterMeshAndName.FindKey(Name);
	if (Mesh)
	{
		return *Mesh;
	}

	return nullptr;
}

FName UCharacterMeshesDataAsset::GetNameByMesh(const USkeletalMesh* Mesh) const
{
	FName const* Name = CharacterMeshAndName.Find(Mesh);
	if (Name)
	{
		return *Name;
	}

	return NAME_None;
}
