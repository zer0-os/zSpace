// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "zSpace/Game/ZSpaceGameInstance.h"

void UWidgetLoadingManagerObject::SetNotShowLoadingWidget(bool NewNotShowLoadingWidget)
{
	bNotShowLoadingWidget = NewNotShowLoadingWidget;	
}

void UWidgetLoadingManagerObject::SetZSpaceGameInstance(UZSpaceGameInstance* NewZSpaceGameInstance)
{
	ZSpaceGameInstance = NewZSpaceGameInstance;	
}

void UWidgetLoadingManagerObject::ShowLoadingWidget(TSubclassOf<UUserWidget> NewLoadingWidget, int32 NewZOrder)
{
	if(bNotShowLoadingWidget)
	{
		return;
	}
	checkf(NewLoadingWidget != nullptr, TEXT("The NewLoadingWidget is nullptr."));
	if(NewLoadingWidget)
	{
		//HideLoadingWidget();
		UGameInstance * GameInstance = UGameplayStatics::GetGameInstance(GetOuter());
		UserWidgetLoading = CreateWidget<UUserWidget>(GameInstance, NewLoadingWidget);
		if(UserWidgetLoading)
		{
			UserWidgetLoading->AddToViewport(NewZOrder);
		}
	}
}

void UWidgetLoadingManagerObject::HideLoadingWidget()
{
	if(IsValid(UserWidgetLoading))
	{
		// RF_BeginDestroyed
		//UserWidgetLoading->BeginDestroy();
		UserWidgetLoading->RemoveFromViewport();
		UserWidgetLoading = nullptr;
	}
}
