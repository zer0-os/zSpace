// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSPlayerController.h"
#include "ZSLoginPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ZSPACE_API AZSLoginPlayerController : public AOWSPlayerController
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscButtonPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetAllCharacters, const TArray<FUserCharacter>&, UserCharacters);

	AZSLoginPlayerController();

	virtual  void BeginPlay() override;

	virtual  void Tick(float NewDeltaSeconds) override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void BindOnGetAllCharacters(const TArray<FUserCharacter>& UserCharacters);
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FString GetUserSessionGUID() const;

	UFUNCTION(BlueprintSetter)
	void SetUserSessionGUID(FString Value);
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FString GetCharacterName() const;

	UFUNCTION(BlueprintSetter)
	void SetCharacterName(FString Value);
	
protected:
	UFUNCTION()
	void OnEscOnClicked();

	void CheckCharacterCountAndAdd(int32 CheckCount, const TArray<FUserCharacter>& UserCharacters);

	UFUNCTION()
	void OnGetAllCharactersEvent(const TArray<FUserCharacter>& UserCharacters);

	/**
	 * @param Length Max Length 32
	 */
	FString GetRandomString(uint8 Length = 12);

public:
	UPROPERTY(BlueprintAssignable)
	FOnEscButtonPressed OnEscButtonPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnGetAllCharacters OnGetAllCharacters;
	
protected:
	UPROPERTY(BlueprintGetter=GetUserSessionGUID, BlueprintSetter=SetUserSessionGUID)
	FString UserSessionGUID;

	UPROPERTY(BlueprintGetter=GetCharacterName, BlueprintSetter=SetCharacterName)
	FString CharacterName;
	
};

