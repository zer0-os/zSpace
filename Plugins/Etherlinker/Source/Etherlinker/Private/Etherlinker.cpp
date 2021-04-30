// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "Etherlinker.h"
#include "EtherlinkerSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"

#define LOCTEXT_NAMESPACE "FEtherlinkerModule"

void FEtherlinkerModule::StartupModule()
{
    RegisterSettings();
}

void FEtherlinkerModule::ShutdownModule()
{
    if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

// Callback for when the settings were saved.
bool FEtherlinkerModule::HandleSettingsSaved()
{
	UEtherlinkerSettings* Settings = GetMutableDefault<UEtherlinkerSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FEtherlinkerModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "Etherlinker",
			LOCTEXT("RuntimeSettingsName", "Etherlinker"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Etherlinker plugin parameters"),
			GetMutableDefault<UEtherlinkerSettings>()
		);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FEtherlinkerModule::HandleSettingsSaved);
		}
	}
}

void FEtherlinkerModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Etherlinker");
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEtherlinkerModule, Etherlinker)