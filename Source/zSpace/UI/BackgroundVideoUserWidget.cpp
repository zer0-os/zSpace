// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/BackgroundVideoUserWidget.h"

#include "zSpace/Components/ManageWidgetsResolution.h"
#include "zSpace/Interfaces/UIResolutionInterface.h"
#include "zSpace/Game/ZSpaceGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "MediaPlayer.h"

void UBackgroundVideoUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UZSpaceGameInstance* GameInstance = GetGameInstance<UZSpaceGameInstance>();
	if (IsValid(GameInstance))
	{
 		ManageWidgetsResolution = GameInstance->GetManageWidgetsResolution();
		if (IsValid(ManageWidgetsResolution))
		{
			if (!ManageWidgetsResolution->OnWidgetChanged.IsAlreadyBound(this, &UBackgroundVideoUserWidget::OnChangedWidget))
			{
				ManageWidgetsResolution->OnWidgetChanged.AddDynamic(this, &UBackgroundVideoUserWidget::OnChangedWidget);		
			}
		}
	}
}

void UBackgroundVideoUserWidget::OnChangedWidget(UUserWidget* Widget, EWidgetType WidgetType)
{
	if (IsValid(Widget))
	{
		if (Widget->Implements<UUIResolutionInterface>())
		{
			const FBackgroundVideo BackgroundVideoStruct = IUIResolutionInterface::Execute_GetBackgroundVideo(Widget);
			CheckVideoAndPlay(BackgroundVideoStruct);
		}
	}
}

void UBackgroundVideoUserWidget::OnEndReachedVideo()
{
	// UKismetSystemLibrary::PrintString(this, "END");
	MediaPlayer->Seek(FTimespan::Zero());
	//MediaPlayer->Reopen();
}

void UBackgroundVideoUserWidget::CheckVideoAndPlay(FBackgroundVideo BackgroundVideoInfo)
{
	if (BackgroundVideoInfo.VideoMaterial != BackgroundVideo->Brush.GetResourceObject())
	{
		BackgroundVideo->Brush.SetResourceObject(BackgroundVideoInfo.VideoMaterial);
		BackgroundVideo->Brush.DrawAs = ESlateBrushDrawType::Image;
	}
	
 	if (BackgroundVideoInfo.MediaPlayer == MediaPlayer)
 	{
 		if (BackgroundVideoInfo.MediaSource != MediaSource)
 		{
 			if (IsValid(MediaPlayer))
 			{
 				MediaPlayer->OpenSource(BackgroundVideoInfo.MediaSource);
 				MediaSource = BackgroundVideoInfo.MediaSource;
 				
 				// UKismetSystemLibrary::PrintString(this, "NEW_1");
 			}
 		}
 	}
	else
	{
		MediaPlayer = BackgroundVideoInfo.MediaPlayer;
		if (IsValid(MediaPlayer))
		{
 			MediaPlayer->OpenSource(BackgroundVideoInfo.MediaSource);
 			MediaSource = BackgroundVideoInfo.MediaSource;
 			// UKismetSystemLibrary::PrintString(this, "NEW_2");
		}
	}

	if (MenuSound != BackgroundVideoInfo.MenuSound)
	{
		MenuSound = BackgroundVideoInfo.MenuSound;
		
		if (IsValid(SoundComponent))
		{
			SoundComponent->Stop();
			SoundComponent->SetSound(MenuSound);
			SoundComponent->Play();
		}
		else
		{
 			SoundComponent = UGameplayStatics::SpawnSound2D(this, MenuSound);
		}
	}

	if (IsValid(MediaPlayer))
	{
		MediaPlayer->OnEndReached.AddUniqueDynamic(this, &UBackgroundVideoUserWidget::OnEndReachedVideo);
	}
}
