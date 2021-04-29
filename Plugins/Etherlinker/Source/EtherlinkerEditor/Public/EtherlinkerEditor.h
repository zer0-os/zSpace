// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class FReply;
class SWidget;

class FEtherlinkerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override;

	// Plugin settings

	void RegisterSettings();

	void UnregisterSettings();

	bool HandleSettingsSaved();

	// Toolbar buttons OnClick events

	void EtherlinkerButtonClicked();

	void StartServerButtonClicked();

	void StopServerButtonClicked();

	void RestartServerButtonClicked();

	void CheckStatusButtonClicked();

	void CompileContractsButtonClicked();
	
	void CompileIntegrationServerButtonClicked();

private:

	// Add toolbar and menu buttons to the editor
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	void FillSubmenu(FMenuBuilder& MenuBuilder);
	TSharedRef<SWidget> FillComboButton(TSharedPtr<class FUICommandList> Commands);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

};