// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>

struct MPriorityQueueNode
{
	bool operator>(const MPriorityQueueNode& Other);
	bool operator<(const MPriorityQueueNode& Other);
	bool operator>=(const MPriorityQueueNode& Other);
	bool operator<=(const MPriorityQueueNode& Other);
};

/**
 *
 */
class MPriorityQueue
{

/*
* Member Functions
*/
public:
	MPriorityQueue();

	bool	IsEmpty() const;
	int32	Num() const;
	void	Swap(MPriorityQueueNode& Other);
	bool	Peek(MPriorityQueueNode& OutNode);

	// Leap에서 부모로 재귀적으로 올려보낸다
	void	Push(MPriorityQueueNode&& InNode);
	// Leap를 Root로 지정 후 내려간다
	bool	Pop(MPriorityQueueNode& OutNode);

private:
	void	Arrange();

	int		GetParentIndex(const int NodeIndex) const;
	int		GetLeftChildIndex(const int NodeIndex) const;
	int		GetRightChildIndex(const int NodeIndex) const;

/*
* Member Variables
*/

private:
/*
* index 0번은 비워둠
* 부모인덱스 * 2 = 왼쪽 자식 인덱스
* 부모 인덱스 * 2 + 1 = 오른쪽 자식 인덱스
* 자식 인덱스 / 2 = 부모인덱스
*/
	std::vector<MPriorityQueueNode> Heap;

	int LeapIndex = -1;
};
