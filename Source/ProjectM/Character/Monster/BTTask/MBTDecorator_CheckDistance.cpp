#include "MBTDecorator_CheckDistance.h"

UMBTDecorator_CheckDistance::UMBTDecorator_CheckDistance()
{
}

bool UMBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Ret = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return Ret;
}
