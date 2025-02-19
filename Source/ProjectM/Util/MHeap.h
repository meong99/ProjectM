#include "CoreMinimal.h"

// 기본이 최대힙(Less)이며 최소힙을 사용할 땐 TGreater를 사용한다.
template <typename T, typename Compare = TLess<T>>
class TMHeap
{
private:
	TArray<T> Data;
	Compare Comp;

	void HeapifyUp(int32 Index)
	{
		while (Index > 0)
		{
			int32 Parent = (Index - 1) / 2;
			if (!Comp(Data[Parent], Data[Index]))
			{
				break;
			}
			Swap(Data[Index], Data[Parent]);
			Index = Parent;
		}
	}

	void heapifyDown(int32 Index)
	{
		int32 Size = Data.Num();
		while (true)
		{
			int32 Left = 2 * Index + 1;
			int32 Right = 2 * Index + 2;
			int32 Best = Index;

			if (Left < Size && Comp(Data[Best], Data[Left]))
			{
				Best = Left;
			}
			if (Right < Size && Comp(Data[Best], Data[Right]))
			{
				Best = Right;
			}
			if (Best == Index)
			{
				break;
			}
			Swap(Data[Index], Data[Best]);
			Index = Best;
		}
	}

	void Swap(T& Left, T& Right)
	{
		T Temp = Left;
		Left = Right;
		Right = Left;
	}

public:
	TMHeap() = default;

	void Push(T Value)
	{
		Data.Add(Value);
		HeapifyUp(Data.size() - 1);
	}
	void Push(T&& Value)
	{
		Data.Add(Value);
		HeapifyUp(Data.size() - 1);
	}

	void Pop()
	{
		if (Data.IsEmpty())
		{
			MCHAE_WARNING("Data is empty");
			return;
		}
		Data[0] = Data[Data.Num() - 1];
		Data.RemoveAt(Data.Num() - 1);
		if (!Data.IsEmpty())
		{
			heapifyDown(0);
		}
	}

	T Top() const
	{
		if (Data.IsEmpty())
		{
			MCHAE_WARNING("Data is empty");
		}
		return Data[0];
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