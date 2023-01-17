// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PackageObjectExplorerStructs.generated.h"

USTRUCT()
struct FObjectExploreEntity
{
	GENERATED_USTRUCT_BODY()
	
	UObject* Object;
	
	TArray<TSharedPtr<FObjectExploreEntity>> Children;
};
