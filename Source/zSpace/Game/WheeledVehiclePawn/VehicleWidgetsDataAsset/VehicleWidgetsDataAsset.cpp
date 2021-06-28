// Copyright 2020 Sabre Dart Studios


#include "VehicleWIdgetsDataAsset.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UVehicleWidgetsDataAsset::GetUserWidgetControlHelper() const
{
	return VehicleWidgets.UserWidgetControlHelperWidget.LoadSynchronous();
}
