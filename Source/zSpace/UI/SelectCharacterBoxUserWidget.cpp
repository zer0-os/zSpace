// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterBoxUserWidget.h"

#include "zSpace/PlayerController/ZSLoginPlayerController.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"
#include "zSpace/Actors/PreviewCharacter.h"
#include <Components/WidgetSwitcher.h>
#include "Components/EditableTextBox.h"
#include "zSpace/UI/ZSCustomButton.h"
#include "zSpace/UI/ZSpaceButton.h"
#include "Kismet/GameplayStatics.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include "zSpace/zSpace.h"


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
		NextCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
		NextCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh);
	}
	if (IsValid(PreviousCharacterMesh))
	{
		PreviousCharacterMesh->SetVisibility(ESlateVisibility::Collapsed);
		PreviousCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh);
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APreviewCharacter::StaticClass(), OutActors);
	if (OutActors.IsValidIndex(0))
	{
		PreviewCharacter = Cast<APreviewCharacter>(OutActors[0]);
	}
}

void USelectCharacterBoxUserWidget::SetupWidget(const FCharacterSelectBoxInfo& CharacterSelectBoxInfo)
{
	PlayerName->SetText(FText::FromString(CharacterSelectBoxInfo.CharacterName));
	PlayerLevel->SetText(FText::FromString(CharacterSelectBoxInfo.CharacterLevel));
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
		
		PC->AddOrUpdateCosmeticCustomCharacterData(UserSessionGUID, CharacterName, MESH_NAME, FieldValue);
		// UKismetSystemLibrary::PrintString(this, FieldValue);
	}
}

void USelectCharacterBoxUserWidget::ChangeCreateCharacterMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = true;
	
	WidgetSwitcherEditMode->SetActiveWidget(RemoveCharacterButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(CreateCharacterDone);

	CreateCharacterNameSwitcher->SetActiveWidget(NewCharacterName);

	// TODO Fix Level 
	PlayerLevel->SetText(FText::FromString("LEVEL 0"));
}

void USelectCharacterBoxUserWidget::ChangeNormalMode()
{
	bIsEditMode = false;
	bIsCreateCharacterMode = false;
	
	WidgetSwitcherEditMode->SetActiveWidget(EditModeButton);
	WidgetSwitcherDoneEditMode->SetActiveWidget(SelectButton);
	CreateCharacterNameSwitcher->SetActiveWidget(PlayerName);

	NewCharacterName->SetText(FText::FromString(""));
}
