// Copyright 2020 Sabre Dart Studios


#include "zSpace/VR/VirtualkeyboarActor/Widgets/ZSVirtualKeyboardUserWidget/ZSVirtualKeyboardUserWidget.h"

void UZSVirtualKeyboardUserWidget::SetWidgetInteractionComponent_Implementation( UWidgetInteractionComponent* NewWidgetInteractionComponent)
{
	WidgetInteractionComponent 	= NewWidgetInteractionComponent;
	OnSetWidgetInteractionComponent.Broadcast(WidgetInteractionComponent);
}

void UZSVirtualKeyboardUserWidget::SetVirtualKeyboardActor_Implementation( AVirtualKeyboardActor* NewVirtualKeyboardActor)
{
	VirtualKeyboardActor = NewVirtualKeyboardActor;
	//OnSetVirtualKeyboardActor.Broadcast(VirtualKeyboardActor);
	EventSetVirtualKeyboardActor();
}


UWidgetInteractionComponent* UZSVirtualKeyboardUserWidget::GetWidgetInteractionComponent_Implementation()
{
	return WidgetInteractionComponent;
}

void UZSVirtualKeyboardUserWidget::SetVirtualKeyboardActorInKey(TArray<UHorizontalBox*> NewHorizontalBox)
{
	for(UHorizontalBox * IterHorizontalBox : NewHorizontalBox)
	{
		TArray<UWidget*> L_UserWidget = IterHorizontalBox->GetAllChildren();
		for(UWidget * IterWidget : L_UserWidget)
		{
			if(IsValid(IterWidget))
			{
				const bool L_IsImplemented = IterWidget->GetClass()->ImplementsInterface(UVirtualKeyboardWidgetInterface::StaticClass());
				if(L_IsImplemented)
				{
					IVirtualKeyboardWidgetInterface::Execute_SetVirtualKeyboardActor(IterWidget, VirtualKeyboardActor);	
				}
			}
		}
	}
}

void UZSVirtualKeyboardUserWidget::SetWidgetInteractionComponentInKey(TArray<UHorizontalBox*> NewHorizontalBox)
{
	for(UHorizontalBox * IterHorizontalBox : NewHorizontalBox)
	{
		TArray<UWidget*> L_UserWidget = IterHorizontalBox->GetAllChildren();
		for(UWidget * IterWidget : L_UserWidget)
		{
			if(IsValid(IterWidget))
			{
				const bool L_IsImplemented = IterWidget->GetClass()->ImplementsInterface(UVirtualKeyboardWidgetInterface::StaticClass());
				if(L_IsImplemented)
				{
					IVirtualKeyboardWidgetInterface::Execute_SetWidgetInteractionComponent(IterWidget, WidgetInteractionComponent);
				}
			}
		}
	}
}
