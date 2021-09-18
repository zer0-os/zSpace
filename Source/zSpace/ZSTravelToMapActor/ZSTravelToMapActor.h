// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSTravelToMapActor.h"
#include "Components/BoxComponent.h"
#include "ZSTravelToMapActor.generated.h"


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
	
protected:
	
	FHttpModule* HttpZS;
	
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
	class AZSCharacterWithAbilities * Character = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AZSWheeledVehiclePawn * Vehicle = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AZSGamePlayerController * PlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class AOWSPlayerState * PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 ShowLoadingDialog:1;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	uint8 UseLineTraceToFindFloor:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSubclassOf<class UUserWidget> UserWidgetLoading;

public:

	void SetCharacter(class AActor * NewOtherActor);
	
	void SetPawn(class AActor * NewOtherActor);

	void SetPlayerController(class AOWSCharacter * NewOWSCharacter, class APawn * Pawn);
	
	void SetPlayerState(class AOWSPlayerController * NewOWSPlayerController );

	bool IsAvailableTravelToMap();

	void DetermineDynamicSpawnAxisAndSetAppropriateOffset();

	UFUNCTION(BlueprintCallable)
	FString GetCharacterName() const ;

	void DisableCharacterMovement();

	UFUNCTION()
	void ComponentBeginOverlap( class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)	
	void ShowLoadingEvent(const FString & NewCharacter);

	UFUNCTION(BlueprintCallable)	
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

	bool IsTeleport(class UPrimitiveComponent * NewOtherComp, class AActor * NewPawn);
	
	UFUNCTION(BlueprintCallable, Category = "Travel")
	void GetZSMapServerToTravelTo(class APlayerController* NewPlayerController, TEnumAsByte<ERPGSchemeToChooseMap::SchemeToChooseMap> NewSelectedSchemeToChooseMap, int32 NewWorldServerID);

	void OnGetZSMapServerToTravelToResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

