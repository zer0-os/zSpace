// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BallisticLineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API UBallisticLineComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallisticLineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 SplinePointAndSplineMeshCount = 20;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USplineComponent * SplineComponentBallisticLine = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UStaticMesh * StaticMeshBallisticLine = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UMaterialInterface * MaterialInterfaceBallisticLine = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TArray<class USplineMeshComponent *> SplineMeshComponentArray;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 bIsCreatedSplineMeshComponent:1;

	uint8 bIsTeleport:1;

	FVector TeleportLocation;

	void CreateSplineComponent();

	void CreateSplineMeshComponents();
	
	void UpdateSplinePoints();

	void UpdateSplineMeshPosition();
	
	void ClearSplineMeshComponent();


public:

	UFUNCTION(BlueprintCallable)
	void ShowBallisticLine();

	UFUNCTION(BlueprintCallable)
	void HideBallisticLine(); 
		
		
};
