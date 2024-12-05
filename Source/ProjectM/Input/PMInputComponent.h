#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PMInputConfig.h"
#include "GameplayTagContainer.h"
#include "PMInputComponent.generated.h"

UCLASS()
class PROJECTM_API UPMInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	/*
	* Overrided Function
	*/
public:
	UPMInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* Member Functions
	*/
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UPMInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UPMInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	/*
	* Member Variables
	*/
};

template<class UserClass, typename FuncType>
void UPMInputComponent::BindNativeAction(const UPMInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
	else
	{
		MCHAE_WARNING("Native 바인딩 실패함!! 태그 확인해야함!");
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UPMInputComponent::BindAbilityActions(const UPMInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FPMInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}