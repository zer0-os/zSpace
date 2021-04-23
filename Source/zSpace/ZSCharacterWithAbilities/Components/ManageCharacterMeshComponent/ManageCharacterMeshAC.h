// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"

#include "ManageCharacterMeshAC.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API UManageCharacterMeshAC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManageCharacterMeshAC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter = nullptr;
	
protected:
	// [All]
	UFUNCTION(BlueprintCallable)
	void UpdateCharacterMesh(const FName& MeshName);

	// [Client]
	UFUNCTION(BlueprintCallable)
	void RequestCharacterMesh();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCharacterMeshesDataAsset* CharacterMeshesDataAsset = nullptr;

public:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdateMesh(class USkeletalMesh* Mesh);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateMesh(class USkeletalMesh* Mesh);
};
