// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterBoxUserWidget.h"

#include "zSpace/BlueprintFunctionLibrary/OWSBlueprintFunctionLibrary.h"
#include "zSpace/PlayerController/ZSLoginPlayerController.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"
#include "zSpace/Actors/PreviewCharacter.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/EditableTextBox.h"
#include "SelectCharacterUserWidget.h"
#include <Components/WidgetSwitcher.h>
#include "zSpace/UI/ZSCustomButton.h"
#include "zSpace/UI/ZSpaceButton.h"
#include "Kismet/GameplayStatics.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/Image.h>


void USelectCharacterBoxUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(EditModeButton))
	{
		EditModeButton->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedEditModeButton);
	}
	if (IsValid(DoneEditModeButton))
	{
		DoneEditModeButton->ButtonWidget->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedDoneEditModeButton);
	}
	if (IsValid(NextCharacterMesh))
	{
		NextCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh);
	}
	if (IsValid(PreviousCharacterMesh))
	{
		PreviousCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh);
	}
	
	// PreviewCharacter = GetPreviewCharacterByEnum(EPreviewCharacterPosition::Middle);
}

void USelectCharacterBoxUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(NextCharacterMesh))
	{
		NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(PreviousCharacterMesh))
	{
		PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(SelectCharacterUserWidget))
	{
		UTexture2D* L_BG = SelectCharacterUserWidget->GetBackGroundImage();
		SetBackGroundImage(L_BG);
	}
}

void USelectCharacterBoxUserWidget::SetPreviewCharacterPosition(EPreviewCharacterPosition NewValue)
{
	PreviewCharacterPosition = NewValue;
	PreviewCharacter = GetPreviewCharacterByEnum(PreviewCharacterPosition);

	if (!IsValid(PreviewCharacter)) return;
	
	auto* RenderTargetAndPosition = PreviewCharacter->GetRenderTargetAndPosition();
	if (IsValid(RenderTargetAndPosition))
	{
		auto Data =  RenderTargetAndPosition->RenderTargetAndPosition;
		UTextureRenderTarget2D* const* RenderTarget = Data.Find(PreviewCharacterPosition);
		if (RenderTarget)
		{
			UMaterialInterface* Mat = Cast<UMaterialInterface>(CharacterRenderImage->Brush.GetResourceObject());
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Mat);
			if (!IsValid(DynamicMaterial))
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(Mat,this, *Mat->GetName());
				CharacterRenderImage->SetBrushFromMaterial(DynamicMaterial);
			}
			if (IsValid(DynamicMaterial))
			{
				DynamicMaterial->SetTextureParameterValue("RenderTarget", *RenderTarget);
			}
		}	
	}
	CharacterInfoForUI.PreviewCharacterDirection = PreviewCharacterPosition;
}

void USelectCharacterBoxUserWidget::SetupWidget(const FCharacterInfoForUI& Data)
{
	PlayerName->SetText(FText::FromString(Data.CharacterName));
	PlayerLevel->SetText(FText::FromString(Data.CharacterLevel));

	CharacterInfoForUI = Data;
}

void USelectCharacterBoxUserWidget::OnClickedEditModeButton()
{
	bIsEditMode = true;
	bIsCreateCharacterMode = false;
	
	WidgetSwitcherEditMode->SetActiveWidget(RemoveCharacterButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(DoneEditModeButton);

	NextCharacterMesh->SetVisibility(ESlateVisibility::Visible);
	PreviousCharacterMesh->SetVisibility(ESlateVisibility::Visible);
}

void USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh()
{
	if (IsValid(PreviewCharacter))
	{
		PreviewCharacter->NextCharacterMesh();
	}
}

void USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh()
{
	if (IsValid(PreviewCharacter))
	{
		PreviewCharacter->PreviousCharacterMesh();
	}
}

void USelectCharacterBoxUserWidget::OnClickedDoneEditModeButton()
{
	if (!IsValid(PreviewCharacter)) return;
	if (!bIsEditMode) return;

	NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	
	WidgetSwitcherDoneEditMode->SetActiveWidget(SelectButton);
	WidgetSwitcherEditMode->SetActiveWidget(EditModeButton);
	bIsEditMode = false;
	
	AZSLoginPlayerController* PC = GetOwningPlayer<AZSLoginPlayerController>();
	if (IsValid(PC))
	{
		const FString UserSessionGUID = PC->GetUserSessionGUID();
		const FString CharacterName = PC->GetCharacterName();
		const FString FieldValue = PreviewCharacter->GetCurrentMeshName().ToString();

		const FString FieldName = UOWSBlueprintFunctionLibrary::GetMeshFieldName(this, CharacterName);
		
		PC->AddOrUpdateCosmeticCustomCharacterData(UserSessionGUID, CharacterName, FieldName, FieldValue);
	}
}

void USelectCharacterBoxUserWidget::ChangeCreateCharacterMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = true;
	
	WidgetSwitcherEditMode->SetActiveWidget(RemoveCharacterButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(CreateCharacterDone);

	CreateCharacterNameSwitcher->SetActiveWidget(NewCharacterName);
	EnterNameBG->SetVisibility(ESlateVisibility::Visible);

	// TODO Fix Level 
	PlayerLevel->SetText(FText::FromString("LEVEL 0"));

	NextCharacterMesh->SetVisibility(ESlateVisibility::Visible);
	PreviousCharacterMesh->SetVisibility(ESlateVisibility::Visible);
	
}

void USelectCharacterBoxUserWidget::ChangeNormalMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = false;
	
	WidgetSwitcherEditMode->SetActiveWidget(EditModeButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(SelectButton);
	CreateCharacterNameSwitcher->SetActiveWidget(PlayerName);

	NewCharacterName->SetText(FText::FromString(""));

	NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
	
	if (PlayerName->GetText().IsEmpty())
	{
		RemoveFromParent();	
	}
	EnterNameBG->SetVisibility(ESlateVisibility::Hidden);
}

void USelectCharacterBoxUserWidget::SetBackGroundImage(class UTexture2D* NewTexture)
{
	if (IsValid(BackgroundImage))
	{
		FSlateBrush BGImageBrush;
		BGImageBrush = BackgroundImage->Brush;

		if(!IsValid(BackgroundImage->Brush.GetResourceObject()))
		{ 
			BGImageBrush.SetResourceObject(NewTexture);
		}
		else
		{
			BGImageBrush.SetResourceObject(CharacterInfoForUI.BackgroundImage);
		}
		BackgroundImage->SetBrush(BGImageBrush);
	}

}

APreviewCharacter* USelectCharacterBoxUserWidget::GetPreviewCharacterByEnum(
	const EPreviewCharacterPosition P_PreviewCharacterPosition) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APreviewCharacter::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		APreviewCharacter* L_PreviewCharacter = Cast<APreviewCharacter>(Actor);
		if (IsValid(L_PreviewCharacter) && L_PreviewCharacter->PreviewCharacterPosition == P_PreviewCharacterPosition)
		{
			return L_PreviewCharacter;
		}
	}

	return nullptr;
}
