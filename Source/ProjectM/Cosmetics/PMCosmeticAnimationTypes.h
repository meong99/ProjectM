#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "PMCosmeticAnimationTypes.generated.h"

class UPhysicsAsset;
class USkeletalMesh;

USTRUCT(BlueprintType)
struct FPMAnimLayerSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequireTags;
};

USTRUCT(BlueprintType)
struct FPMAnimLayerSelectionSet
{
	GENERATED_BODY()

	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPMAnimLayerSelectionEntry> LayerRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
};

USTRUCT(BlueprintType)
struct FPMAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// 적용할 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// 어떤 메쉬인지 구분짓기위한 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FPMAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	// 태그로 구분지어진 메쉬. 만약 여기서 적용시킬 메쉬를 못 찾는다면 Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPMAnimBodyStyleSelectionEntry> MeshRules;

	// 적용시킬 메쉬를 못 찾는다면 DefaultMesh를 적용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// 모든 Physics Asset은 하나로 정해져있다. 모든 애니메이션은 동일한 Physics속성을 공유한다.
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};