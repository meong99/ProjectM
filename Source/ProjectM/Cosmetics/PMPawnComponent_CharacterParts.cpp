#include "PMPawnComponent_CharacterParts.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

/*
* FPMCharacterPartHandle
*/

FPMCharacterPartList::FPMCharacterPartList()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance() &&
		IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

FPMCharacterPartList::FPMCharacterPartList(UPMPawnComponent_CharacterParts* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
	if (IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

FPMCharacterPartHandle FPMCharacterPartList::AddEntry(FPMCharacterPart NewPart)
{
	FPMCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FPMAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		if (SpawnActorForEntry(NewEntry))
		{
			// Actor가 생성 및 적용되었으니 Animation 및 Physics를 Re-initialize한다
			OwnerComponent->BroadcastChanged();
		}
	}
	else
	{
		ensure(false);
		MCHAE_WARNING("Something wrong with OwnerComponent! Check OwnerComponent!");
	}

	return Result;
}

bool FPMCharacterPartList::SpawnActorForEntry(FPMAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	if (Entry.Part.PartClass != nullptr)
	{
		UWorld* World = OwnerComponent->GetWorld();

		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			//지금 사용은 안 함
// 			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);

			//Render World에 적용시킨다
			PartComponent->RegisterComponent();

			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					// remind : Tick이 도는 순서를 정하는 구간이다. 부모 틱이 먼저 돌 수 있도록 보장해준다.
					// RootComponent -> CharactarPart의 순서를 지정하는 것
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}

			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActor = true;
		}
	}

	return bCreatedAnyActor;
}

void FPMCharacterPartList::RemoveEntry(FPMCharacterPartHandle Handle)
{
	for (auto EntryIter = Entries.CreateIterator(); EntryIter; ++EntryIter)
	{
		FPMAppliedCharacterPartEntry& Entry = *EntryIter;

		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}

void FPMCharacterPartList::DestroyActorForEntry(FPMAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FGameplayTagContainer FPMCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	for (const FPMAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent)
		{
			/*
			* 해당 Actor의 IGameplayTagAssetInterface를 통해 GameplayTag를 검색한다.
			* 현재 우리의 TagActor는 IGameplayTagAssetInterface를 상속받지 않으므로 그냥 넘어갈 것이다.
			* 나중에 각 Part에 대해 GameplayTag를 넣고 싶다면 이걸 상속받아 정의해야한다.
			* 예를들어 특정 Lv100이상 장착 가능한 장비를 만들고 싶다면 넣어야 겠다.
			*/
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}

	return Result;
}

/*
* UPMPawnComponent_CharacterParts
*/
UPMPawnComponent_CharacterParts::UPMPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
	, CharacterPartList(this) // <-------------------- 이 부분이 중요하다!!! this로 CharacterPartList에게 Owner를 지정해줘야한다!
{
	SetIsReplicatedByDefault(true);
}

void UPMPawnComponent_CharacterParts::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterPartList);
}

USkeletalMeshComponent* UPMPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}

	return nullptr;
}

USceneComponent* UPMPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}

	if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}

	return nullptr;
}

void UPMPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// 여기서 원하는 태그를 넣으면 그 태그에 맞는 메쉬를 들고와서 설정을 진행한다.
		// 하지만 지금은 태그를 사용하지 않고 기본 메쉬만 사용하기 때문에 우선은 아래와 같이 진행한다.
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag{});
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}

FPMCharacterPartHandle UPMPawnComponent_CharacterParts::AddCharacterPart(const FPMCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void UPMPawnComponent_CharacterParts::RemoveCharacterPart(FPMCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

FGameplayTagContainer UPMPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}

	return Result;
}