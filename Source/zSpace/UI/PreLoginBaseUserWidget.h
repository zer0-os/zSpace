// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "ZSpaceButton.h"
#include "Blueprint/UserWidget.h"
#include "zSpace/Interfaces/UIResolutionInterface.h"

#include "PreLoginBaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSPACE_API UPreLoginBaseUserWidget : public UUserWidget, public IUIResolutionInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickedSignIn();
	
	UFUNCTION()
	void OnClickedRegister();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSpaceButton* SignInButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UZSpaceButton* RegisterButton = nullptr;

	UPROPERTY(BlueprintReadOnly)
	class UManageWidgetsResolution* ManageWidgetsResolution = nullptr;

public:
	virtual EWidgetType GetWidgetType_Implementation() override;
	
};
