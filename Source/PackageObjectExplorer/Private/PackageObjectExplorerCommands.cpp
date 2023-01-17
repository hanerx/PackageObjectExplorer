// Copyright Epic Games, Inc. All Rights Reserved.

#include "PackageObjectExplorerCommands.h"

#define LOCTEXT_NAMESPACE "FPackageObjectExplorerModule"

void FPackageObjectExplorerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PackageObjectExplorer", "Bring up PackageObjectExplorer window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
