// Copyright 2020 Sabre Dart Studios

#include "zSpace/EthereumTerminalComponent/Widgets/ZSWalletBalanceUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "zSpace/EthereumTerminalComponent/EthereumTerminalComponent.h"
#include "zSpace/Game/ZSPlayerState/Components/ZSEtherManager/ZSEtherManager.h"

void UZSWalletBalanceUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UZSWalletBalanceUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(nullptr != TextBlockBalance)
	{
		TextBlockBalance->SetText(FText());
	}
}


void UZSWalletBalanceUserWidget::GetBalance(UEthereumTerminalComponent* NewEthereumTerminalComponent)
{
	if(IsValid(NewEthereumTerminalComponent))
	{
		AZSGamePlayerController * PC = GetPlayerController();
		if(IsValid(PC))
		{
			PC->UseTerminal(NewEthereumTerminalComponent);
			UZSEtherManager * L_ZsEtherManager = GetZsEtherManager();
			if(IsValid(L_ZsEtherManager))
			{
				L_ZsEtherManager->OnClientResponseReceivedEvent.AddUniqueDynamic(this, &UZSWalletBalanceUserWidget::ResponseReceived);
			}
		}
	}
}

UZSEtherManager* UZSWalletBalanceUserWidget::GetZsEtherManager()
{
	UZSEtherManager * R_ZsEtherManager = nullptr;
	AController * PC =	GetPlayerController();
	if(IsValid(PC))
	{
		const bool IsImplemented = PC->GetClass()->ImplementsInterface(UEtherlinkerPCInterface::StaticClass());
		if(IsImplemented)
		{
			R_ZsEtherManager = IEtherlinkerPCInterface::Execute_GetZSEtherManager(PC);	
			UE_LOG(LogTemp, Log, TEXT("Got ZSEtherManager from PC"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The PlayerController doesn't implemented IEtherlinkerPCInterface"));
		}
	}
	return R_ZsEtherManager;
}

AZSGamePlayerController * UZSWalletBalanceUserWidget::GetPlayerController()
{
	AZSGamePlayerController * PC = Cast<AZSGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	return PC;
}

void UZSWalletBalanceUserWidget::ResponseReceived(FString Result, FEtherlinkerResponseData Data)
{
	FString L_Out;
	FJsonObjectConverter::UStructToJsonObjectString(Data, L_Out);
	TextBlockBalance->SetText(FText::FromString(L_Out));
	UE_LOG(LogTemp, Warning, TEXT("UserWidget ZSpace: Data =  %s"), *L_Out);
}
