// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSPlayerController.h"
#include "ZSPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ZSPACE_API AZSPlayerController : public AOWSPlayerController
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscButtonPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetAllCharacters, const TArray<FUserCharacter>&, UserCharacters);

	AZSPlayerController();

	virtual  void BeginPlay() override;

	virtual  void Tick(float NewDeltaSeconds) override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void BindOnGetAllCharacters(const TArray<FUserCharacter>& UserCharacters);
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FString GetUserSessionGUID() const;

	UFUNCTION(BlueprintSetter)
	void SetUserSessionGUID(FString Value);
	
	
protected:
	UFUNCTION()
	void OnEscOnClicked();

	void CheckCharacterCountAndAdd(int32 CheckCount, const TArray<FUserCharacter>& UserCharacters);

	UFUNCTION()
	void OnGetAllCharactersEvent(const TArray<FUserCharacter>& UserCharacters);

public:
	UPROPERTY(BlueprintAssignable)
	FOnEscButtonPressed OnEscButtonPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnGetAllCharacters OnGetAllCharacters;
	
protected:
	UPROPERTY(BlueprintGetter=GetUserSessionGUID, BlueprintSetter=SetUserSessionGUID)
	FString UserSessionGUID;
	
};

