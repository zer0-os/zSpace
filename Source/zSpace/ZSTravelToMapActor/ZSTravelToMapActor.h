// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSTravelToMapActor.h"
#include "Components/BoxComponent.h"
#include "ZSTravelToMapActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowLoadingDialogEvent, FString, CharacterName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHideLoadingDialogEvent, FString, CharacterName);

UCLASS()
class ZSPACE_API AZSTravelToMapActor : public AOWSTravelToMapActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZSTravelToMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class USceneComponent * SceneComponentRoot = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UBoxComponent * BoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FVector PointToMoveTo;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float AxisOffset;

	float ZAxisOffset;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AOWSCharacter * Character = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AOWSPlayerController * PlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AOWSPlayerState * PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 ShowLoadingDialog:1;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 UseLineTraceToFindFloor:1;

public:

	void GetOWSCharacter(class AActor * NewOtherActor);

	void GetPlayerController(class AOWSCharacter * NewOWSCharacter);
	
	void GetPlayerState(class AOWSPlayerController * NewOWSPlayerController );

	bool IsAvailableTravelToMap();

	void DetermineDynamicSpawnAxisAndSetAppropriateOffset();

	UFUNCTION(BlueprintCallable)
	FString GetCharacterName() const ;

	void DisableCharacterMovement();
	

	UFUNCTION()
	void ComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintAssignable)	
	FShowLoadingDialogEvent OnShowLoadingDialogEvent;

	UPROPERTY(BlueprintAssignable)	
	FHideLoadingDialogEvent OnHideLoadingDialogEvent;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)	
	void ShowLoadingEvent(const FString & NewCharacter);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)	
	void HideLoadingEvent(const FString & NewCharacter);

	// TODO EventNotifyMapServerToTravelTo and EventErrorMapServerToTravelTo function is TMP
	UFUNCTION(BlueprintCallable)	
	void EventNotifyMapServerToTravelTo(const FString &ServerAndPort);
	
	UFUNCTION(BlueprintCallable)	
	void EventErrorMapServerToTravelTo(const FString &ErrorMsg);

	void CalculatePointToMoveTo();

	FRotator GetCharacterSpawnRotation();

	FVector DetectGroundLocation();

	UFUNCTION()
	void ResetState();
	
};

