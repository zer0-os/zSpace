// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "CharacterUnderFootSurfaceDA/CharacterUnderFootSurfaceDA.h"
#include "Components/ActorComponent.h"
#include "DetectSurfaceTypeComponent.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class ECharacterFootType: uint8
{
	  NONE  UMETA(DisplayName = "None")
	, LEFT  UMETA(DisplayName = "Left")
	, RIGHT UMETA(DisplayName = "Right") 
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API UDetectSurfaceTypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetectSurfaceTypeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="BoneName")
	FString LeftFootBoneName = "foot_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true), Category="BoneName")
	FString RightFootBoneName = "foot_r";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UCharacterUnderFootSurfaceDA * CharacterUnderFootSurfaceDA = nullptr;

	USkeletalMeshComponent * GetSkeletalMesh() const;
	
	FVector GetFootLocationByCharacterFootType(ECharacterFootType NewCharacterFootType);

	void PlayRandomSound(FCharacterUnderFootSurfaceData NewCharacterUnderFootSurfaceData, const FVector & NewLocation);
	
public:	

	UFUNCTION(BlueprintCallable)
	void PutFootOnGround(ECharacterFootType NewCharacterFootType);
		
};

