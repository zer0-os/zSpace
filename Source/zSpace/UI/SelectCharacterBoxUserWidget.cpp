// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/SelectCharacterBoxUserWidget.h"

#include "zSpace/PlayerController/ZSLoginPlayerController.h"
#include "zSpace/Types/CharacterMeshesDataAsset.h"
#include "zSpace/Actors/PreviewCharacter.h"
#include <Components/WidgetSwitcher.h>
#include "Components/EditableTextBox.h"
#include "SelectCharacterUserWidget.h"
#include "zSpace/UI/ZSCustomButton.h"
#include "zSpace/UI/ZSpaceButton.h"
#include "Kismet/GameplayStatics.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "Net/UnrealNetwork.h"
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
		NextCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedNextCharacterMesh);
	}
	if (IsValid(PreviousCharacterMesh))
	{
		PreviousCharacterMesh->OnClicked.AddUniqueDynamic(this, &USelectCharacterBoxUserWidget::OnClickedPreviousCharacterMesh);
	}
	
	PreviewCharacter = GetPreviewCharacterByEnum(EPreviewCharacterPosition::Middle);
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
}

void USelectCharacterBoxUserWidget::OnRep_PreviewCharacterPosition()
{
	// UKismetSystemLibrary::PrintString(this, "++++++++++++");
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

void USelectCharacterBoxUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USelectCharacterBoxUserWidget, PreviewCharacterPosition);
}
