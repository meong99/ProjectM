// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerStateComponentBase.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Table/MTable_LevelTable.h"
#include "MLevelComponent.generated.h"

class UPMAbilitySystemComponent;

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UMLevelComponent : public UMPlayerStateComponentBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMLevelComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnExperienceLoaded(const UPMExperienceDefinition* LoadedExperienceDefinition) override;
	virtual void InitializeComponent() override;

	/*
* Member Functions
*/
public:
	
protected:
	void SetMaxExperiencePoint(const int32 Level);

	void OnChange_ExperiencePoint(FGameplayTag Tag, const FGameplayEventData* EventData);

	void LevelUp();

	UFUNCTION()
	void OnRep_OnChangeLevel(const int32 OldLevel);
	UFUNCTION()
	void OnRep_OnChangeExperience(const int64 OldExperiencePoint);

#if WITH_EDITOR
	UFUNCTION(Exec)
	void Debug_LevelUp();
	UFUNCTION(Server, Reliable)
	void Debug_LevelUpServer();
#endif
/*
* Member Variables
*/

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_OnChangeLevel", BlueprintReadOnly, meta = (ClampMin = 0), Category = "ProjectM")
	int32	CurrentLevel = 1;

	UPROPERTY(ReplicatedUsing = "OnRep_OnChangeExperience", BlueprintReadOnly, meta = (ClampMin = 0), Category = "ProjectM")
	int64	CurrentExperiencePoint = 0;

	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = 0.0), Category="ProjectM")
	int64	CurrentMaxExperiencePoint = 1;

	UPROPERTY()
	UPMAbilitySystemComponent* AbilitySystemComp;

	FMTable_LevelTable* CurrentLevelTableRow;
};
