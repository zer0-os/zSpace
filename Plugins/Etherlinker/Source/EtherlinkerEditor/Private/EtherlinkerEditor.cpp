// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "EtherlinkerEditor.h"
#include "EtherlinkerStyle.h"
#include "EtherlinkerCommands.h"
#include "EtherlinkerSettings.h"
#include "EtherlinkerFunctionLibrary.h"
#include "Misc/MessageDialog.h"
#include "SlateCore/Public/Widgets/SWidget.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"

static const FName ToolbarTestTabName("Etherlinker");

#define LOCTEXT_NAMESPACE "FEtherlinkerEditorModule"

void FEtherlinkerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEtherlinkerStyle::Initialize();
	FEtherlinkerStyle::ReloadTextures();

	FEtherlinkerCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().EtherlinkerAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::EtherlinkerButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().StartServerAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::StartServerButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().StopServerAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::StopServerButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().RestartServerAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::RestartServerButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().CheckServerStatusAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::CheckStatusButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().CompileContractsAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::CompileContractsButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FEtherlinkerCommands::Get().CompileServerAction,
		FExecuteAction::CreateRaw(this, &FEtherlinkerEditorModule::CompileIntegrationServerButtonClicked),
		FCanExecuteAction());
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("LevelEditor", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FEtherlinkerEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::Before, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FEtherlinkerEditorModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FEtherlinkerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (UEtherlinkerFunctionLibrary::CheckIntegrationServer()) {
		UEtherlinkerFunctionLibrary::StopIntegrationServer();
	}

	FEtherlinkerStyle::Shutdown();

	FEtherlinkerCommands::Unregister();
}

bool FEtherlinkerEditorModule::SupportsDynamicReloading()
{
	return true;
}

void FEtherlinkerEditorModule::EtherlinkerButtonClicked()
{
	/*
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FEtherlinkerEditorModule::PluginButtonClicked()")),
		FText::FromString(TEXT("ToolbarTest.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	*/
	UE_LOG(LogTemp, Warning, TEXT("EtherlinkerButtonClicked"));
}

void FEtherlinkerEditorModule::StartServerButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("StartServerButtonClicked"));
	FProcHandle ProcessHandle;
	bool Result = UEtherlinkerFunctionLibrary::StartIntegrationServer();
	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server started");
	}
	else {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server already started");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

}

void FEtherlinkerEditorModule::StopServerButtonClicked()
{
	bool Result = UEtherlinkerFunctionLibrary::StopIntegrationServer();
	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server stopped");
	}
	else {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server is not started");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FEtherlinkerEditorModule::RestartServerButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("RestartServerButtonClicked"));
	bool Result = UEtherlinkerFunctionLibrary::RestartIntegrationServer();
	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server successfully restarted");
	}
	else {
		DialogText = LOCTEXT("StartServerButtonClicked", "Can't restart integration server");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FEtherlinkerEditorModule::CheckStatusButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckStatusButtonClicked"));
	bool Result = UEtherlinkerFunctionLibrary::CheckIntegrationServer();
	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server is running");
	}
	else {
		DialogText = LOCTEXT("StartServerButtonClicked", "Integration server is not running");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FEtherlinkerEditorModule::CompileContractsButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("CompileContractsButtonClicked"));

	bool Result;
	if (UEtherlinkerFunctionLibrary::CompileContracts()) {
		Result = UEtherlinkerFunctionLibrary::CompileIntegrationServer();
	}
	else Result = false;

	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("CompileContractsButtonClicked", "Contracts compiled successfully. Server recompiled successfully.");
	}
	else {
		DialogText = LOCTEXT("CompileContractsButtonClicked", "Contracts is not compiled successfully. Server is not recompiled. See output log for more details.");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

}

void FEtherlinkerEditorModule::CompileIntegrationServerButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("CompileIntegrationServerButtonClicked"));

	bool Result;
	Result = UEtherlinkerFunctionLibrary::CompileIntegrationServer();

	FText DialogText;
	if (Result) {
		DialogText = LOCTEXT("CompileContractsButtonClicked", "Integration server successfully compiled.");
	}
	else {
		DialogText = LOCTEXT("CompileContractsButtonClicked", "Server is not compiled. See output log for more details.");
	}
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FEtherlinkerEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.BeginSection("EtherlinkerMenu");

	Builder.AddSubMenu(FText::FromString("Etherlinker"),
		FText::FromString("Interact with integration server"),
		FNewMenuDelegate::CreateRaw(this, &FEtherlinkerEditorModule::FillSubmenu),
		false,
		FSlateIcon(FEtherlinkerStyle::GetStyleSetName(), "Etherlinker.EtherlinkerAction"));

	Builder.EndSection();
}

void FEtherlinkerEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	FUIAction TempCompileOptionsCommand;

	Builder.AddComboButton(
		TempCompileOptionsCommand,
		FOnGetContent::CreateRaw(this, &FEtherlinkerEditorModule::FillComboButton, PluginCommands),
		LOCTEXT("EtherlinkerComboButton", "Etherlinker"),
		LOCTEXT("EtherlinkerComboButtonTootlip", "Interact with integration server"),
		FSlateIcon(FEtherlinkerStyle::GetStyleSetName(), "Etherlinker.EtherlinkerAction"),
		false
	);

}

void FEtherlinkerEditorModule::FillSubmenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().StartServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().StopServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().RestartServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CheckServerStatusAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CompileServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CompileContractsAction);
}

TSharedRef<SWidget> FEtherlinkerEditorModule::FillComboButton(TSharedPtr<class FUICommandList> Commands)
{
	FMenuBuilder MenuBuilder(true, Commands);

	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().StartServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().StopServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().RestartServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CheckServerStatusAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CompileServerAction);
	MenuBuilder.AddMenuEntry(FEtherlinkerCommands::Get().CompileContractsAction);

	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEtherlinkerEditorModule, EtherlinkerEditor)