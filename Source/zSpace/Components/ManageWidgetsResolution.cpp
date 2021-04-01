// Copyright 2020 Sabre Dart Studios


#include "zSpace/Components/ManageWidgetsResolution.h"

#include "../Interfaces/UIResolutionInterface.h"
#include <GameFramework/PlayerController.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/UserWidget.h>

// Sets default values for this component's properties
UManageWidgetsResolution::UManageWidgetsResolution() {}

bool UManageWidgetsResolution::CreateWidgetAndAddViewprot(APlayerController* PlayerControler, TSubclassOf<class UUserWidget> WidgetSubClass, EResolution Resolution, UUserWidget*& ReturnWidget)
{
	if (!WidgetSubClass) return false;

	UUserWidget* WidgetDefaultObject = WidgetSubClass.GetDefaultObject();
	if (IsValid(WidgetDefaultObject))
	{
		bool IsImplemented = WidgetDefaultObject->Implements<UUIResolutionInterface>();
		if (IsImplemented)
		{
			EWidgetType WidgetType = IUIResolutionInterface::Execute_GetWidgetType(WidgetDefaultObject);
			UUserWidget* Widget = GetWidgetByResolution(WidgetType, Resolution);
			if (IsValid(Widget))
			{
				if (!Widget->IsInViewport())
				{
					ReturnWidget = Widget;
					Widget->AddToViewport();
				}

				// UKismetSystemLibrary::PrintString(this, GetNameSafe(Widget));
				UKismetSystemLibrary::PrintString(this, Widget->IsConstructed() ? "11" : "22");
				
				UKismetSystemLibrary::PrintString(this, "++++++++++");
				return true;
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, "-----------");
				bool OutResult;
				TMap<EResolution, UUserWidget*>& Widgets = GetWidgetsByEnum(WidgetType, OutResult);
				UUserWidget* CreatedWidget = CreateWidget(PlayerControler, WidgetSubClass);
				if (IsValid(CreatedWidget))
				{
					Widgets.Add(Resolution, CreatedWidget);
					CreatedWidget->AddToViewport();
					ReturnWidget = CreatedWidget;
					return true;
				}
			}
		}
	}
	return false;
}

TMap<EResolution, class UUserWidget*>& UManageWidgetsResolution::GetWidgetsByEnum(EWidgetType WidgetType, bool& Result)
{
	switch (WidgetType)
	{
	case EWidgetType::PreLogin:
		Result = true;
		return PreLoginWidgets;
		break;
	case EWidgetType::Login:
		Result = true;
		return LoginWidgets;
		break;
	default:
		Result = false;
		return EmptyWidgets;
		break;
	}
}

UUserWidget* UManageWidgetsResolution::GetWidgetByResolution(EWidgetType WidgetType, EResolution Resolution)
{
	bool GetWidgetsByEnumResult = false;
	TMap<EResolution, UUserWidget*> Widgets = GetWidgetsByEnum(WidgetType, GetWidgetsByEnumResult);
	if (GetWidgetsByEnumResult)
	{
		UUserWidget** FindWidget = Widgets.Find(Resolution);
		if (FindWidget)
		{
			return *FindWidget;
		}
	}

	return nullptr;
}

