// Copyright Epic Games, Inc. All Rights Reserved.

#include "PackageObjectExplorer.h"

#include "ContentBrowserModule.h"
#include "PackageObjectExplorerStyle.h"
#include "PackageObjectExplorerCommands.h"
#include "LevelEditor.h"
#include "SPackageObjectExplorer.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "UnrealExporter.h"

static const FName PackageObjectExplorerTabName("PackageObjectExplorer");

#define LOCTEXT_NAMESPACE "FPackageObjectExplorerModule"

void FPackageObjectExplorerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPackageObjectExplorerStyle::Initialize();
	FPackageObjectExplorerStyle::ReloadTextures();

	FPackageObjectExplorerCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);


	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(
		"ContentBrowser");
	{
		FContentBrowserMenuExtender_SelectedAssets ContentBrowserDelegate =
			FContentBrowserMenuExtender_SelectedAssets::CreateLambda([this](const TArray<FAssetData>& SelectedAssets)
			{
				TSharedRef<FExtender> Extender = MakeShareable(new FExtender());
				Extender->AddMenuExtension(
					"CommonAssetActions",
					EExtensionHook::Before,
					PluginCommands,
					FMenuExtensionDelegate::CreateLambda([this, SelectedAssets](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.AddMenuEntry(FPackageObjectExplorerCommands::Get().OpenPluginWindow);
						PluginCommands->MapAction(
							FPackageObjectExplorerCommands::Get().OpenPluginWindow,
							FExecuteAction::CreateLambda([this, SelectedAssets]()
							{
								this->PluginButtonClicked(SelectedAssets);
							}),
							FCanExecuteAction());
					}));
				return Extender;
			});
		TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = ContentBrowserModule.
			GetAllAssetViewContextMenuExtenders();
		CBMenuExtenderDelegates.Add(ContentBrowserDelegate);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PackageObjectExplorerTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FPackageObjectExplorerModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FPackageObjectExplorerTabTitle", "PackageObjectExplorer"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPackageObjectExplorerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPackageObjectExplorerStyle::Shutdown();

	FPackageObjectExplorerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PackageObjectExplorerTabName);
}

TSharedRef<SDockTab> FPackageObjectExplorerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SPackageObjectExplorer)
		];
}

void FPackageObjectExplorerModule::PluginButtonClicked(const TArray<FAssetData>& SelectedAssets) const
{
	if(const TSharedPtr<SDockTab> NewTab = FGlobalTabmanager::Get()->TryInvokeTab(PackageObjectExplorerTabName))
	{
		const TSharedRef<SPackageObjectExplorer> PackageObjectExplorer = StaticCastSharedRef<SPackageObjectExplorer>(NewTab->GetContent());
		PackageObjectExplorer->SetAssetData(SelectedAssets);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPackageObjectExplorerModule, PackageObjectExplorer)
