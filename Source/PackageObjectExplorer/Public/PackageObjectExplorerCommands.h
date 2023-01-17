// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PackageObjectExplorerStyle.h"

class FPackageObjectExplorerCommands : public TCommands<FPackageObjectExplorerCommands>
{
public:

	FPackageObjectExplorerCommands()
		: TCommands<FPackageObjectExplorerCommands>(TEXT("PackageObjectExplorer"), NSLOCTEXT("Contexts", "PackageObjectExplorer", "PackageObjectExplorer Plugin"), NAME_None, FPackageObjectExplorerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};