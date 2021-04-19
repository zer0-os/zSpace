// Copyright 2020 Sabre Dart Studios


#include "zSpace/Game/WidgetLoadingManagerObject/WidgetLoadingManagerObject.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UWidgetLoadingManagerObject::ShowLoadingWidget(TSubclassOf<UUserWidget> NewLoadingWidget, int32 NewZOrder)
{
	checkf(NewLoadingWidget != nullptr, TEXT("The NewLoadingWidget is nullptr."));
	if(NewLoadingWidget)
	{
		HideLoadingWidget();
		APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
		UserWidgetLoading = CreateWidget<UUserWidget>(PC, NewLoadingWidget);
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
