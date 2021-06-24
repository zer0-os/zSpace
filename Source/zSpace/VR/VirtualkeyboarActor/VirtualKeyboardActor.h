// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VirtualKeyboardActor.generated.h"

UENUM(BlueprintType)
enum class ESpecialKey : uint8
{
	 NORMAL  UMETA(DisplayName = "Normal")
	,SPECIAL UMETA(DisplayName = "Special") 
};

USTRUCT(BlueprintType)
struct FVirtualKeyboardKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpecialKey IsSpecialKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey SpecialKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShiftPressedKey;

	
};

UCLASS()
class ZSPACE_API AVirtualKeyboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVirtualKeyboardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class USceneComponent * RootSceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UWidgetComponent * WidgetComponentVirtualKeyboard = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	uint8 bIsPressedShift:1;

	friend class UZSKeyboardKeyUserWidget;
	friend class UZSVirtualKeyboardUserWidget;

	
public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressedShift, const bool, NewIsShiftPressed);

	FPressedShift OnPressedShift;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void SetVirtualKeyboardActor();

	UFUNCTION(BlueprintCallable)
	void SetIsPressedShift(const bool NewIsPressedShift);
	
};
