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

	UPROPERTY(ReplicatedUsing=OnRep_CharacterMesh)
	class USkeletalMesh* CharacterMesh = nullptr;

	UFUNCTION()
	void OnRep_CharacterMesh();

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterMesh(class USkeletalMesh* Mesh);

	UFUNCTION(BlueprintCallable)
	void SetMesh(class USkeletalMesh* Mesh);

protected:
	virtual void GetLifetimeReplicatedProps(TArray <class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
