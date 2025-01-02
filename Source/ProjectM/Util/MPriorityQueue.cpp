#include "MPriorityQueue.h"

bool MPriorityQueueNode::operator>(const MPriorityQueueNode& Other)
{
	return false;
}

bool MPriorityQueueNode::operator<(const MPriorityQueueNode& Other)
{
	return false;
}

bool MPriorityQueueNode::operator>=(const MPriorityQueueNode& Other)
{
	return false;
}

bool MPriorityQueueNode::operator<=(const MPriorityQueueNode& Other)
{
	return false;
}

MPriorityQueue::MPriorityQueue()
{
	// Index 0번은 비워둠
	Heap.push_back({});
}

bool MPriorityQueue::IsEmpty() const
{
	return Num() == 0;
}

int32 MPriorityQueue::Num() const
{
	return (int32)Heap.size() - 1;
}

void MPriorityQueue::Swap(MPriorityQueueNode& Other)
{

}

bool MPriorityQueue::Peek(MPriorityQueueNode& OutNode)
{
	if (IsEmpty())
	{
		return false;
	}

	OutNode = Heap[1];
}

void MPriorityQueue::Push(MPriorityQueueNode&& InNode)
{
	Heap.push_back(InNode);

	Arrange();
}

bool MPriorityQueue::Pop(MPriorityQueueNode& OutNode)
{
	if (IsEmpty())
	{
		return false;
	}

	auto Iter = Heap.erase(Heap.begin() + 1, Heap.begin() + 1);
	OutNode = *Iter;
	return true;
}

void MPriorityQueue::Arrange()
{

}

int MPriorityQueue::GetParentIndex(const int NodeIndex) const
{
	if (NodeIndex * 0.5 > 0)
	{
		return (int)(NodeIndex * 0.5);
	}

	return 0;
}

int MPriorityQueue::GetLeftChildIndex(const int NodeIndex) const
{
	return NodeIndex * 2;
}

int MPriorityQueue::GetRightChildIndex(const int NodeIndex) const
{
	return NodeIndex * 2 + 1;
}
