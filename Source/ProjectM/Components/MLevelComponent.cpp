#include "MLevelComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "PMGameplayTags.h"
#include "System/MDataTableManager.h"
#include "Table/MTable_LevelTable.h"

UMLevelComponent::UMLevelComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UMLevelComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMLevelComponent, CurrentLevel);
	DOREPLIFETIME_CONDITION(UMLevelComponent, CurrentExperiencePoint, COND_OwnerOnly);
}

void UMLevelComponent::OnExperienceLoaded(const UPMExperienceDefinition* LoadedExperienceDefinition)
{
	AbilitySystemComp = GetAbilitySystemComponent<UPMAbilitySystemComponent>();
	if (AbilitySystemComp)
	{
		AbilitySystemComp->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer(FPMGameplayTags::Get().GameplayEvent_Character_ChangeExp),
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UMLevelComponent::OnChange_ExperiencePoint));
	}
	else
	{
		MCHAE_ERROR("AbilitySystem is not initialized!!! Level system requires gameplay event!!")
		check(false);
	}
}

void UMLevelComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetMaxExperiencePoint(CurrentLevel);
}

void UMLevelComponent::OnChange_ExperiencePoint(FGameplayTag Tag, const FGameplayEventData* EventData)
{
	if (AbilitySystemComp && EventData)
	{
		CurrentExperiencePoint += EventData->EventMagnitude;
	}
}

void UMLevelComponent::SetMaxExperiencePoint(const int32 Level)
{
	int32 RowId = UMDataTableManager::MakeRowId((int32)EMTableKey::LevelTable, Level);
	FMTable_LevelTable* TableRow = UMDataTableManager::GetTableRowData<FMTable_LevelTable>(this, RowId);
	if (TableRow)
	{
		CurrentMaxExperiencePoint = TableRow->MaxExperiencePoint;
	}
}

void UMLevelComponent::OnRep_OnChangeLevel(const int32 OldLevel)
{
	if (OldLevel < CurrentLevel)
	{

	}

	SetMaxExperiencePoint(CurrentLevel);
}

void UMLevelComponent::OnRep_OnChangeExperience(const int64 OldExperiencePoint)
{

}
