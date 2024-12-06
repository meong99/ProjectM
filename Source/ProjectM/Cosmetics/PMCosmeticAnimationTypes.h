#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "PMCosmeticAnimationTypes.generated.h"

class UPhysicsAsset;
class USkeletalMesh;

USTRUCT(BlueprintType)
struct FPMAnimInstanceSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequireTags;
};

// 각 특성에 맞는 AnimInstance를 들고있는 곳
USTRUCT(BlueprintType)
struct FPMAnimInstanceSelectionSet
{
	GENERATED_BODY()

	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPMAnimInstanceSelectionEntry> AnimInstanceRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultAnimInstance;
};

USTRUCT(BlueprintType)
struct FPMBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// 적용할 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// 어떤 메쉬인지 구분짓기위한 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

// 애니메이션을 지정하는게 아닌 메쉬를 지정하는 곳이다.
USTRUCT(BlueprintType)
struct FPMBodyStyleSelectionSet
{
	GENERATED_BODY()

	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	// 태그로 구분지어진 메쉬. 만약 여기서 적용시킬 메쉬를 못 찾는다면 Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPMBodyStyleSelectionEntry> MeshRules;

	// 적용시킬 메쉬를 못 찾는다면 DefaultMesh를 적용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// 강제로 적용하고싶은 Physics. 만약 이게 존재하면 CharacterPart에 있는 Physics를 사용하지 않고 아래를 사용한다.
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};