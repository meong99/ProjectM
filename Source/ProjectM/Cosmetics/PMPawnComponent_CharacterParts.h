#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "PMCharacterPartTypes.h"
#include "PMCosmeticAnimationTypes.h"

#include "PMPawnComponent_CharacterParts.generated.h"

class UPMPawnComponent_CharacterParts;

// 실제 적용된 Character Part에 대한 Data
USTRUCT()
struct FPMAppliedCharacterPartEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FPMCharacterPart Part;

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// Actor를 Actor에게 붙일 땐 UChildActorComponent를 사용해야하며, 실제 스폰되어 부착된 Object이다.
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/*
* Unreal의 네트워크 방식은 어떤 데이터를 보낼 때 배열 중 하나만 바뀌어도 전체를 다 보내는 형태로 되어있기 때문에 너무 무겁다.
* 그래서 Network Serialize라는 방식을 이용해서 수정된 부분만 보내는 형태로 만든게 아래처럼 List, Entry로 배열과 아이템을 나눠서 데이터를 구분지을 수 있게 만든거다.
* 이 방식을 사용하기 위해서는 List는 FFastArraySerializer로, Item은 FFastArraySerializerItem를 상속받아서 선언하면 최적화를 할 수 있다.
*/
// Instancing된 Character Parts를 관리하는 구조체
USTRUCT(BlueprintType)
struct FPMCharacterPartList
{
	GENERATED_BODY()

	FPMCharacterPartList()
		: OwnerComponent(nullptr)
	{
	}

	// CharacterPartList는 Owner가 꼭 필요하다!
	FPMCharacterPartList(UPMPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	// 실질적으로 CharacterPart가 저장, 적용되는 부분이다.
	FPMCharacterPartHandle AddEntry(FPMCharacterPart NewPart);

	bool SpawnActorForEntry(FPMAppliedCharacterPartEntry& Entry);

	void RemoveEntry(FPMCharacterPartHandle Handle);
	void DestroyActorForEntry(FPMAppliedCharacterPartEntry& Entry);

	FGameplayTagContainer CollectCombinedTags() const;

	UPROPERTY()
	TArray<FPMAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UPMPawnComponent_CharacterParts> OwnerComponent;

	int32 PartHandleCounter = 0;
};

// Character에 부착되는 CharacterPart를 들고있을 Component.
UCLASS(meta = (BlueprintSpawnableComponent))
class PROJECTM_API UPMPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
	: Super(ObjectInitializer)
	, CharacterPartList(this) // <-------------------- 이 부분이 중요하다!!! this로 CharacterPartList에게 Owner를 지정해줘야한다!
	{}

public:
/*
* Member Functions
*/
	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;
	void BroadcastChanged();

	FPMCharacterPartHandle AddCharacterPart(const FPMCharacterPart& NewPart);
	void RemoveCharacterPart(FPMCharacterPartHandle Handle);

private:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Cosmetics", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
/*
* Member Variables
*/

private:
	//Instancing된 CharacterParts
	UPROPERTY()
	FPMCharacterPartList CharacterPartList;

	//애니메이션 적용을 위핸 메시와 연결고리
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
	FPMAnimBodyStyleSelectionSet BodyMeshes;
};