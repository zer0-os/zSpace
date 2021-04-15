#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Types/UITypes.h"
#include "BackgroundVideoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UBackgroundVideoUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override; 

	UFUNCTION()
	void OnChangedWidget(class UUserWidget* Widget, EWidgetType WidgetType);
	
	UFUNCTION()
	void OnEndReachedVideo();
	
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UImage* BackgroundVideo = nullptr;

	UPROPERTY()
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaPlayer* MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMediaSource* MediaSource = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* MenuSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAudioComponent* SoundComponent = nullptr;
		
public:
	UFUNCTION(BlueprintCallable)
	void CheckVideoAndPlay(FBackgroundVideo BackgroundVideoInfo);
	
};
