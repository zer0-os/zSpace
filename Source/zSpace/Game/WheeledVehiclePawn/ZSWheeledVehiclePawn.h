// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "WheeledVehiclePawn.h"
#include "zSpace/Game/ZSCameraComponent/ZSCameraComponent.h"
#include "GameplayEffectTypes.h"
#include "Accessories/SteeringWheelStaticMeshComponent/SteeringWheelStaticMeshComponent.h"
#include "ZSWheeledVehiclePawn.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EVehicleLiveDirection :uint8
{
	 LEFT_DIRECTION UMETA(DisplayName = "Left Direction")
	,RIGHT_DIRECTION UMETA(DisplayName = "Right Direction")
};

/**
 * 
 */
UCLASS()
class ZSPACE_API AZSWheeledVehiclePawn : public AWheeledVehiclePawn, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
private:
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	//class UPathFollowingComponent * PathFollowingComponent = nullptr;
	
public:



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))	
	class USteeringWheelStaticMeshComponent * SteeringWheelStaticMeshComponent = nullptr;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))	
	class USkeletalMeshComponent * SkeletalMeshComponentDriver = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	EVehicleLiveDirection VehicleLiveDirection = EVehicleLiveDirection::LEFT_DIRECTION;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities", meta = (AllowPrivateAccess=true))	
	class UAbilitySystemComponent * AbilitySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CharacterBase")
	const class UZSVehicleAttributeSet * AttributeSetVehicle = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSSpringArmComponent *  SpringArmComponentDefault = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSCameraComponent * CameraComponentDefault = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSSpringArmComponent *  SpringArmComponentInSide = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UZSCameraComponent * CameraComponentInSide = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UAudioComponent * EngineSoundComponent = nullptr; 

	UFUNCTION(BlueprintCallable)
	class UZSVehicleMovementComponent * GetZSVehicleMovementComponent() const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UBoxComponent * VehicleZoneBoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class AZSCharacterWithAbilities * 	ZSCharacterWithAbilities = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UWidgetComponent * Speedometer3D = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FName> DriverBoneNameForHHide;
	
	UPROPERTY(BlueprintReadOnly, meta =(AllowPrivateAccess=true))
	uint8 IsTransferringBetweenMaps:1;

	UPROPERTY(BlueprintReadOnly, meta =(AllowPrivateAccess=true))
	uint8 bIsEngineStarted:1;

	UPROPERTY(ReplicatedUsing=OnRepHiddenDriver, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 bIsHiddenDriver:1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USurfaceTypesDetectActorComponent * SurfaceTypesDetectActorComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USpawnTrackComponent * SpawnTrackComponent = nullptr;

	FTimerHandle TimerHandleTurnOff;

	FTimerDelegate TimerDelegateTurnOff;
	
public:

	UFUNCTION()
	void SetIsHiddenDriver(bool  NewIsHiddenDriver);
	

	// -1..0..1
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess=true)) 
	float ForwardInput;
	
	// -1..0..1
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess=true)) 
	float SteeringInput;

	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USpotLightComponent * SpotLightComponentFrontLeftLight = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USpotLightComponent * SpotLightComponentFrontRightLight = nullptr;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetForwardInputValue(const float & NewForwardInput);

	void SendForwardInputToServer(const float & NewForwardInput );
	
	AZSWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetZsCharacterWithAbilities(class AZSCharacterWithAbilities * 	NewZSCharacterWithAbilities);

	
	UFUNCTION(BlueprintCallable)
	class AZSCharacterWithAbilities * GetCharacterWithAbilities();

	UFUNCTION(BlueprintCallable)
	void HideVehicleControlWidget();
	
	UFUNCTION(BlueprintCallable)
	void ShowVehicleControlWidget();
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UVehicleWidgetsDataAsset * VehicleWidgetsDataAsset = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * VehicleControlWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	ECameraPositionType SelectedCameraPositionType = ECameraPositionType::DefaultCamera;

	static FName VehicleStopLightParamName;
	
	static FName VehicleRearLightParamName;

	static FName VehicleFrontAndRearLightsParamName;

	static FName VehicleInSideLight;
		
protected:
	virtual void BeginPlay() override;
	
public:
	
	virtual void Tick(float DeltaSeconds) override;


	
protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnRectifyVehicle();
	
	UFUNCTION(BlueprintCallable)
	bool IsEnterVehicle(AActor * NewOtherActor);
	
	UFUNCTION()
	void ComponentBeginOverlapVehicleZoneBoxComponent(UPrimitiveComponent * OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void  ComponentEndOverlapVehicleZoneBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual UAbilitySystemComponent * GetAbilitySystemComponent() const override;

public:

	UFUNCTION()
	void MoveForward(float NewValue);

	UFUNCTION()
	void MoveRight(float NewValue);

	void SendMoveRightValue(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMoveRightValue(const float & NewValue);
	
	UFUNCTION()
	void HandbrakePressed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HandbrakePressed();
	
	UFUNCTION()
	void HandbrakeReleased();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HandbrakeReleased();

	UFUNCTION()
	void LookUp(float NewValue);

	UFUNCTION()
	void LookRight(float NewValue);
	
	UFUNCTION()
	void LeaveVehicle();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeaveVehicle();
	


	UFUNCTION(BlueprintCallable)
	void SetupDefaultCamera(ECameraPositionType NewCameraPositionType);
	
	UFUNCTION(BlueprintCallable)
	bool IsExistCamera(ECameraPositionType NewCameraPositionType);

	UFUNCTION(BlueprintCallable)
	void VehicleNextCamera();
	
	UFUNCTION(BlueprintCallable)
	void VehicleBackCamera();

	void InitAttributes();


	// Attributes for HealthBody
	UFUNCTION(BlueprintPure, Category="VehicleBase")
	void GetHealthBody(float & HealthBody, float & MaxHealth);
	
	// Attributes for HealthBody
	UFUNCTION(BlueprintPure, Category="VehicleBase")
	void GetHealthEngine(float &HealthEngine, float & MaxHealthEngine );
	
	// Attributes for Gas Tank
	UFUNCTION(BlueprintPure, Category="VehicleBase")
	void GetGasTank(float & GasTank, float & MaxGasTank);

	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnHealthBodyChanged(float  OldValue, float  NewValue);
	
	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnHealthEngineChanged(float OldValue, float NewValue );
	
	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnGasTankChanged(float  OldValue, float NewValue);
	
	void OnHealthBodyChangedNative(const FOnAttributeChangeData & NewData);
	
	void OnHealthEngineChangedNative(const FOnAttributeChangeData & NewData );
	
	void OnGasTankChangedNative(const FOnAttributeChangeData & NewData);

	void OnStopRearLightChangedNative(const FOnAttributeChangeData & NewData);
	
	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnStopRearLightChanged(float  OldValue, float NewValue);
	
	void OnRearLightChangedNative(const FOnAttributeChangeData & NewData);
	
	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnRearLightChanged(float  OldValue, float NewValue);

	void OnFrontAndRearLightChangedNative(const FOnAttributeChangeData & NewData);
	
	UFUNCTION(BlueprintImplementableEvent, Category="VehicleBase")
	void OnFrontAndRearLightChanged(float  OldValue, float NewValue);
	
	void StopRearLight(const FOnAttributeChangeData& NewData);
	
	void RearLight(const FOnAttributeChangeData& NewData);

	void FrontRearLights(const FOnAttributeChangeData& NewData);	

	void EnableFrontLight();

	UFUNCTION(Server, Reliable, WithValidation)	
	void Server_EnableFrontLight();

private:

	UFUNCTION()	
	void OnRepHiddenDriver();


public:
	
	UFUNCTION(BlueprintCallable)
	void InitializeAbility(TSubclassOf<class UGameplayAbility> NewAbilityToGet, int32 AbilityLevel);

	void CheckVehicleStop();

	virtual void UnPossessed() override;
	
	virtual void PossessedBy(AController* NewController) override;

	void OnFrontLights(const  bool & IsEnableLights);

	void OnInSideLight(const  bool & IsEnableLights);

	class USteeringWheelStaticMeshComponent * GetSteeringWheelStaticMeshComponent();

	bool SkipComponent(UPrimitiveComponent * NewComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateSpringLimitationByCameraComponent(class UZSCameraComponent * NewCameraComponent);

	void ShowDriverHead(const ECameraPositionType & NewCameraPositionType);

	UFUNCTION(Client, Reliable)
	void Client_SetupDefaultCamera();

	void EngineAudioProcessing();

	UFUNCTION(NetMulticast, Reliable)
	void SetEngineStart(bool NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastEnableTick(bool  NewEnable);

	UFUNCTION()
	void SetDriverSkeletalMesh(AZSCharacterWithAbilities * NewZSCharacterWithAbilities);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UPROPERTY()
	class USkeletalMesh * SkeletalMeshDriver = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_DriverSkeletalMeshName)
	FString DriverSkeletalMeshName;

	UFUNCTION()
	void OnRep_DriverSkeletalMeshName();

	class USkeletalMesh * GetCharacterSkeletalMeshBySkeletalMeshName(const FString& NewSkeletalMeshName);
	
public:
	
	UFUNCTION(NetMulticast, Reliable)
	void SetIdleBrakeInput(float  NewInput);


#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void EnableMove();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void DisableMove();

	UFUNCTION(BlueprintPure)
	bool IsTickWorks() const;

	UFUNCTION(BlueprintCallable)
	void TurnOffVehicle();
	
	UFUNCTION(BlueprintCallable)
	void TurnOnVehicle();

	UFUNCTION()
	void CanDisableVehicleMovement();

	UFUNCTION(BlueprintCallable)
	void SwitchToCharacter();

	UFUNCTION(BlueprintCallable)
	void SaveVehicleModelOfCharacter();


	void DisableClientCollisions();
	
};

