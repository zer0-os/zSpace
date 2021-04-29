// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

#include "EtherlinkerCommands.h"

#define LOCTEXT_NAMESPACE "FEtherlinkerModule"

void FEtherlinkerCommands::RegisterCommands()
{
	UI_COMMAND(EtherlinkerAction, "Etherlinker", "Interact with integration server", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(StartServerAction, "Start integration server", "Start integration server", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(StopServerAction, "Stop integration server", "Stop integration server", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(RestartServerAction, "Restart integration server", "Restart integration server", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CheckServerStatusAction, "Check integration server status", "Check integration server status", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CompileContractsAction, "Compile contracts", "Compile new contracts and recompile integration server", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CompileServerAction, "Compile integration server", "Compile integration server", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
