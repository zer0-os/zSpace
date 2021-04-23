// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/ManageCharacterMeshComponent/ManageCharacterMeshAC.h"

#include "OWSPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "zSpace/Game/ZSpaceGameInstance.h"

// Sets default values for this component's properties
UManageCharacterMeshAC::UManageCharacterMeshAC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UManageCharacterMeshAC::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (!IsValid(OwnerCharacter)) return;
		
		RequestCharacterMesh();
	}
}

void UManageCharacterMeshAC::UpdateCharacterMesh(const FName& MeshName)
{
	if (!IsValid(CharacterMeshesDataAsset)) return;
	
	auto* Mesh = CharacterMeshesDataAsset->GetMeshByName(MeshName);
	if (IsValid(Mesh))
	{
		Server_UpdateMesh(Mesh);
	}
}

void UManageCharacterMeshAC::RequestCharacterMesh()
{
	if (GetOwnerRole() != ROLE_AutonomousProxy) return;
	
	AOWSPlayerController* PC = OwnerCharacter->GetController<AOWSPlayerController>();
	if (!IsValid(PC)) return;

	UZSpaceGameInstance* GameInstance = Cast<UZSpaceGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!IsValid(GameInstance)) return;

	const FString UserSessionGUID = GameInstance->UserSessionGUID;
	const FString CharacterName = GameInstance->CharacterName;

	PC->GetAllCharacters(UserSessionGUID);
	PC->GetCosmeticCustomCharacterData(UserSessionGUID, CharacterName);
}

void UManageCharacterMeshAC::Server_UpdateMesh_Implementation(USkeletalMesh* Mesh)
{
	NetMulticast_UpdateMesh(Mesh);
}

bool UManageCharacterMeshAC::Server_UpdateMesh_Validate(USkeletalMesh* Mesh)
{
	return true;
}

void UManageCharacterMeshAC::NetMulticast_UpdateMesh_Implementation(USkeletalMesh* Mesh)
{
	if (IsValid(Mesh))
	{
		if (IsValid(OwnerCharacter))
		{
			USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
			if (IsValid(CharacterMesh))
			{
				CharacterMesh->SetSkeletalMesh(Mesh);
			}
		}
	}
}
