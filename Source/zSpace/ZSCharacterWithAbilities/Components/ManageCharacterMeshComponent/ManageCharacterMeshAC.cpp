// Copyright 2020 Sabre Dart Studios


#include "zSpace/ZSCharacterWithAbilities/Components/ManageCharacterMeshComponent/ManageCharacterMeshAC.h"

#include "OWSPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
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

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter)) return;
	
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		RequestCharacterMesh();
	}

	if (IsValid(CharacterMesh))
	{
		SetMesh(CharacterMesh);
	}
}

void UManageCharacterMeshAC::UpdateCharacterMesh(const FName& MeshName)
{
	if (!IsValid(CharacterMeshesDataAsset)) return;
	
	auto* Mesh = CharacterMeshesDataAsset->GetMeshByName(MeshName);
	if (IsValid(Mesh))
	{
		Server_SetCharacterMesh(Mesh);
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

void UManageCharacterMeshAC::OnRep_CharacterMesh()
{
	// FString Text = "OnRep_" + GetNameSafe(CharacterMesh);

	// ENetRole R = GetOwnerRole();
	// switch (R)
	// {
	// case ROLE_SimulatedProxy: UKismetSystemLibrary::PrintString(this, Text + "   Simulated", true, true, FLinearColor::Red, 25.f); break;
	// case ROLE_AutonomousProxy: UKismetSystemLibrary::PrintString(this, Text + "   Autonomous", true, true, FLinearColor::Red, 25.f); break;
	// case ROLE_Authority: UKismetSystemLibrary::PrintString(this, Text + "   Server", true, true, FLinearColor::Red, 25.f); break;
	// }

	
	SetMesh(CharacterMesh);
}

void UManageCharacterMeshAC::SetMesh(USkeletalMesh* Mesh)
{
	if (IsValid(Mesh))
	{
		if (IsValid(OwnerCharacter))
		{
			USkeletalMeshComponent* L_CharacterMesh = OwnerCharacter->GetMesh();
			if (IsValid(L_CharacterMesh))
			{
				L_CharacterMesh->SetSkeletalMesh(Mesh);
			}
		}
	}
}

void UManageCharacterMeshAC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UManageCharacterMeshAC, CharacterMesh);
	DOREPLIFETIME_CONDITION_NOTIFY(UManageCharacterMeshAC, CharacterMesh, COND_None, REPNOTIFY_Always);
}

void UManageCharacterMeshAC::Server_SetCharacterMesh_Implementation(USkeletalMesh* Mesh)
{
	CharacterMesh = Mesh;
}

bool UManageCharacterMeshAC::Server_SetCharacterMesh_Validate(USkeletalMesh* Mesh)
{
	return true;
}

