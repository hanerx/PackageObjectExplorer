// Copyright Epic Games, Inc. All Rights Reserved.

#include "PackageObjectExplorerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FPackageObjectExplorerStyle::StyleInstance = nullptr;

void FPackageObjectExplorerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FPackageObjectExplorerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FPackageObjectExplorerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("PackageObjectExplorerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FPackageObjectExplorerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("PackageObjectExplorerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("PackageObjectExplorer")->GetBaseDir() / TEXT("Resources"));

	Style->Set("PackageObjectExplorer.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FPackageObjectExplorerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FPackageObjectExplorerStyle::Get()
{
	return *StyleInstance;
}
