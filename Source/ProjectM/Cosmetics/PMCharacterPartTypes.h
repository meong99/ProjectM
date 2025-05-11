#pragma once

#include "Templates/SubclassOf.h"
#include "PMCharacterPartTypes.generated.h"

class AActor;

// Possess하고있는 Pawn에 대해 Instancing된 Character Part의 핸들.
USTRUCT(BlueprintType)
struct FPMCharacterPartHandle 
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

// Character Part에 대한 MetaData(가벼운 데이터)
USTRUCT(BlueprintType)
struct FPMCharacterPart
{
	GENERATED_BODY()

	// 부착시킬 Actor Class를 들고있는다.
	// 이 Actor는 캐릭터가 될 수도 있고, 무기가 될 수도 있고, 다른 특정 부품이 될 수도 있다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// 부착시킬 Socket. None이라면 Root에 붙인다
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};