#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"

#include "MInventoryTypes.generated.h"

class UPMInventoryItemInstance;
class UPMInventoryItemDefinition;

UENUM(BlueprintType)
enum class EMItemType : uint8
{
	Equipment,
	Consumable,
	Others,

	None,
};

USTRUCT(BlueprintType)
struct FMItemHandle 
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return ItemUid != INDEX_NONE;
	}

	UPROPERTY(BlueprintReadWrite)
	int32 ItemUid = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	EMItemType ItemType = EMItemType::None;
};

UENUM(BlueprintType)
enum class EMItemRequestType : uint8
{
	None,

	// 초기화할 때 추가되는 아이템 1~n개
	InitItem,

	// 아이템 추가 1~n개
	AddItem,

	// 아이템 제거 1~n개
	RemoveItem,

	// 이미 인스턴스 존재하는 아이템이 인벤토리로 복귀. 1~n개. Request에 ItemInstance 추가 필수
	ReturnItemToInventory,
};

USTRUCT(BlueprintType)
struct FMItemRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EMItemRequestType RequestType = EMItemRequestType::None;

	// ItemDef가 있으면 ItemRowId를 사용하지 않음.
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UPMInventoryItemDefinition> ItemDef;

	// ItemDef == nullptr이라면 RowId를 통해 아이템 탐색
	UPROPERTY(BlueprintReadWrite)
	int32 ItemRowId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemQuentity = 0;

	UPROPERTY(BlueprintReadWrite)
	FMItemHandle ItemHandle;

	// 장비 장착 해제 등 이미 인스턴스가 존재하는 아이템을 인벤토리로 되돌리는 형태의 요청일때만 사용. 서버-서버, 클라-클라만 사용
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPMInventoryItemInstance> ItemInstance;
};

UENUM(BlueprintType)
enum class EMItemResponseType : uint8
{
	None,
	TotallyNewItem,
	ChangeItemQuentity,
	Removed,
};

USTRUCT(BlueprintType)
struct FMItemResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FMItemRequest ItemRequest;
	
	UPROPERTY(BlueprintReadWrite)
	EMItemResponseType ResponsType = EMItemResponseType::None;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemQuentity;
};
