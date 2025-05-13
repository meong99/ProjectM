// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "MDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = EditorPerProjectUserSettings)
class PROJECTMEDITOR_API UMDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	UMDeveloperSettings();
/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
#if WITH_EDITORONLY_DATA
	/** A list of common maps that will be accessible via the editor detoolbar */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category=Maps, meta=(AllowedClasses="/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonEditorMaps;
#endif
};
