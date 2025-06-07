#include "MLevelComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "PMGameplayTags.h"
#include "System/MDataTableManager.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"

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

void UMLevelComponent::SetMaxExperiencePoint(const int32 Level)
{
	int32 RowId = UMDataTableManager::MakeRowId((int32)EMTableKey::LevelTable, Level);
	CurrentLevelTableRow = UMDataTableManager::GetTableRowData<FMTable_LevelTable>(this, RowId);
	if (CurrentLevelTableRow)
	{
		CurrentMaxExperiencePoint = CurrentLevelTableRow->MaxExperiencePoint;
	}
}

void UMLevelComponent::OnChange_ExperiencePoint(FGameplayTag Tag, const FGameplayEventData* EventData)
{
	if (AbilitySystemComp && EventData)
	{
		CurrentExperiencePoint += EventData->EventMagnitude;
		if (CurrentExperiencePoint >= CurrentMaxExperiencePoint)
		{
			LevelUp();
		}
	}
}

void UMLevelComponent::LevelUp()
{
	CurrentLevel++;
	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		OnRep_OnChangeLevel(CurrentLevel - 1);
	}

	int64 OldExp = CurrentExperiencePoint;
	CurrentExperiencePoint = CurrentExperiencePoint - CurrentMaxExperiencePoint;
	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		OnRep_OnChangeExperience(OldExp);
	}
}

void UMLevelComponent::OnRep_OnChangeLevel(const int32 OldLevel)
{
	if (OldLevel < CurrentLevel)
	{
		ACharacter* Character = GetPawn<ACharacter>();
		if (Character && CurrentLevelTableRow)
		{
			if (CurrentLevelTableRow->LevelUpParticle)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					CurrentLevelTableRow->LevelUpParticle,
					Character->GetMesh(),
					FName("root"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					FVector(.5f),
					EAttachLocation::Type::SnapToTargetIncludingScale,
					true,
					ENCPoolMethod::AutoRelease
				);
			}

			if (CurrentLevelTableRow->LevelUpSound)
			{
				CurrentLevelTableRow->LevelUpSound;
			}
		}
	}

	SetMaxExperiencePoint(CurrentLevel);
}

void UMLevelComponent::OnRep_OnChangeExperience(const int64 OldExperiencePoint)
{

}

void UMLevelComponent::Debug_LevelUp()
{
	Debug_LevelUpServer();
}

void UMLevelComponent::Debug_LevelUpServer_Implementation()
{
	LevelUp();
}
