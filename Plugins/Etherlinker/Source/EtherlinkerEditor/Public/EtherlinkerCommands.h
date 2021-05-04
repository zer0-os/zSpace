// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EtherlinkerStyle.h"

class FEtherlinkerCommands : public TCommands<FEtherlinkerCommands>
{
public:

	FEtherlinkerCommands()
		: TCommands<FEtherlinkerCommands>(TEXT("Etherlinker"), NSLOCTEXT("Contexts", "Etherlinker", "Etherlinker Plugin"), NAME_None, FEtherlinkerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > EtherlinkerAction;
	TSharedPtr< FUICommandInfo > CompileContractsAction;
	TSharedPtr< FUICommandInfo > StartServerAction;
	TSharedPtr< FUICommandInfo > StopServerAction;
	TSharedPtr< FUICommandInfo > RestartServerAction;
	TSharedPtr< FUICommandInfo > CheckServerStatusAction;
	TSharedPtr< FUICommandInfo > CompileServerAction;
};
