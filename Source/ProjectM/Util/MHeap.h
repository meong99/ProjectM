#include "CoreMinimal.h"

// 기본이 최대힙(Less)이며 최소힙을 사용할 땐 TGreater를 사용한다.
template <typename T, typename Compare = TLess<T>>
class TMHeap
{
private:
	TArray<T>	Data;
	Compare		Comp;
	
public:
	TMHeap()
	{
		Data.Heapify(Comp);
	}

	void Push(T Value)
	{
		Data.HeapPush(Value, Comp);
	}
	void Push(T&& Value)
	{
		Data.HeapPush(Value, Comp);
	}

	T Pop()
	{
		T Item;
		Data.HeapPop(Item, Comp);

		return Item;
	}

	T Top() const
	{
		if (Data.IsEmpty())
		{
			MCHAE_WARNING("Data is empty");
			return;
		}

		return Data.HeapTop();
	}

	bool Empty() const
	{
		return Data.IsEmpty();
	}

	int32 Size() const
	{
		return Data.Num();
	}
};