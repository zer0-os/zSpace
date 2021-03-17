// Copyright 2018 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h" 
#include "OWSAttributeSet.h"
//#include "OWSPawnMovementComponent.h"
#include "OWSPawnWithAbilities.generated.h"

class UCapsuleComponent;
class UOWSPawnMovementComponent;

UCLASS()
class OWSPLUGIN_API AOWSPawnWithAbilities : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOWSPawnWithAbilities();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UOWSPawnMovementComponent* OWSPawnMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override
	{
		return Cast<UPawnMovementComponent>(OWSPawnMovementComponent);
	};

	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName MeshComponentName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE class UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	/** Name of the CapsuleComponent. */
	static FName CapsuleComponentName;

	static FName AbilitySystemComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		UOWSAttributeSet* OWSAttributes;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystem;
	};

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;

	/** Get the saved translation offset of mesh. This is how much extra offset is applied from the center of the capsule. */
	UFUNCTION(BlueprintCallable, Category = Character)
		FVector GetBaseTranslationOffset() const { return BaseTranslationOffset; }

	/** Get the saved rotation offset of mesh. This is how much extra rotation is applied from the capsule rotation. */
	virtual FQuat GetBaseRotationOffset() const { return BaseRotationOffset; }

	void CacheInitialMeshOffset(FVector MeshRelativeLocation, FRotator MeshRelativeRotation);

	virtual void PostInitializeComponents() override;

protected:
	/** Saved translation offset of mesh. */
	UPROPERTY()
		FVector BaseTranslationOffset;

	/** Saved rotation offset of mesh. */
	UPROPERTY()
		FQuat BaseRotationOffset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;

};
