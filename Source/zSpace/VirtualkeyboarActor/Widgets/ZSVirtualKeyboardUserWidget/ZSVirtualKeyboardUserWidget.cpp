// Copyright 2020 Sabre Dart Studios


#include "zSpace/VirtualkeyboarActor/Widgets/ZSVirtualKeyboardUserWidget/ZSVirtualKeyboardUserWidget.h"

void UZSVirtualKeyboardUserWidget::SetWidgetInteractionComponent_Implementation( UWidgetInteractionComponent* NewWidgetInteractionComponent)
{
	WidgetInteractionComponent 	= NewWidgetInteractionComponent;
	OnSetWidgetInteractionComponent.Broadcast(WidgetInteractionComponent);
}


UWidgetInteractionComponent* UZSVirtualKeyboardUserWidget::GetWidgetInteractionComponent_Implementation()
{
	return WidgetInteractionComponent;
}

