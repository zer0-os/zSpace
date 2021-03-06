// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"
#include "OWSGameInstance.h"
#include "OWSPlayerController.h"
#include "ZSpaceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UZSpaceGameInstance : public UOWSGameInstance
{
	GENERATED_BODY()

public:

	
	UZSpaceGameInstance(const FObjectInitializer& ObjectInitializer);

	// [Client]
	UFUNCTION(BlueprintCosmetic)
	void InitManageWidgetsResolution();

	UFUNCTION(BlueprintPure)
	class UManageWidgetsResolution* GetManageWidgetsResolution() const;

	// [Client]
	UFUNCTION(BlueprintCosmetic)
	void CreateWidgetLoadingManagerObject();
	
	UFUNCTION(BlueprintPure)
	class UWidgetLoadingManagerObject * GetWidgetLoadingManagerObject() const ;

	UFUNCTION(BlueprintPure)
	class USoundManager * GetSoundManagerObject() const ;

public:
	
	virtual void Init() override;

	UFUNCTION()
	void BeginLoadingLevel(const FString& MapName);
	
	void EndLoadingLevel(UWorld* NewWorld);

	UFUNCTION(BlueprintCallable)
	void HideOrShowGamplayWidget();

	UPROPERTY(BlueprintReadWrite)
	bool bIsHUDVisible = true;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UManageWidgetsResolution> ManageWidgetsResolutionSubClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

	/**
	 * @brief The WidgetLoadingManagerObjectSubClass manager of loading widget.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UWidgetLoadingManagerObject> WidgetLoadingManagerObjectSubClass;
	
	/**
	 * @brief The WidgetLoadingManagerObject pointer of WidgetLoadingManagerObject
	 */
	UPROPERTY(BlueprintReadOnly, Category="UI")
	class UWidgetLoadingManagerObject * WidgetLoadingManagerObject = nullptr;

	/**
	 * @brief The LoadingWidget is Widget 'sub class' that shows loading effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UUserWidget> LoadingWidgetSubClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TSubclassOf<class USoundManager> SoundManagerClass;

	UPROPERTY(BlueprintReadOnly)
	class USoundManager* SoundManagerRef;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Sessions")
	FString CharacterName;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UUserWidget> CreateWalletWidgetSoftClassPtr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * CreateWalletWidget = nullptr;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void ShowCreateWalletWidget();

	UFUNCTION(BlueprintCallable)
	void HideCreateWalletWidget();

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UUserWidget> LoadRemoteWalletWidgetSoftClassPtr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * LoadRemoteWalletWidget = nullptr;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void ShowLoadRemoteWalletWidget();

	UFUNCTION(BlueprintCallable)
	void HideLoadRemoteWalletWidget();
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	TSoftClassPtr<class UUserWidget> LoadWalletWithMnemonicWidgetSoftClassPtr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UUserWidget * LoadWalletWithMnemonicWidget = nullptr;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void ShowLoadWalletWithMnemonicWidget();

	UFUNCTION(BlueprintCallable)
	void HideLoadWalletWithMnemonicWidget();

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	int32 CurrentSelectedCharacterIndex = -1;

	UPROPERTY()
	FTimerHandle TimerHandleLoadLevel;

	UPROPERTY()
	FTimerHandle TimerHandleEndLoadingLevel;

	UPROPERTY()
	TArray<FUserCharacter> UserCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UCharacterMeshesDataAsset * CharacterMeshesDataAsset = nullptr;
	
public:

	UFUNCTION(BlueprintPure)
	class UCharacterMeshesDataAsset * GetCharacterMeshesDataAsset() const;
	

	UFUNCTION(BlueprintCallable)
	void SetCurrentSelectedCharacterIndex(const int32 & NewCurrentSelectedCharacterIndex);

	UFUNCTION()
	void NotifyGetAllUserCharacters(const TArray<struct FUserCharacter> & NewUserCharacter);

	UFUNCTION()
	void StreamingLevelLoaded();
	
};
