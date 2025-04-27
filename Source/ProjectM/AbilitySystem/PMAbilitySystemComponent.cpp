#include "PMAbilitySystemComponent.h"
#include "Abilities/PMGameplayAbility.h"
#include "Animation/PMAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"

UPMAbilitySystemComponent::UPMAbilitySystemComponent()
{
}

void UPMAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && InAvatarActor != ActorInfo->AvatarActor;

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		if (UPMAnimInstance* AnimInst = Cast<UPMAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			AnimInst->InitializeWithAbilitySystem(this);
		}
	}
}

void UPMAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UPMAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UPMAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpenHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UPMAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpenHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UPMAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// 입력 홀드 관련
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpenHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UPMGameplayAbility* AbilityCDO = CastChecked<UPMGameplayAbility>(AbilitySpec->Ability);

				if (AbilityCDO->GetActivationPolicy() == EPMAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// 입력 눌렀을 때 트리거 관련
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UPMGameplayAbility* AbilityCDO = CastChecked<UPMGameplayAbility>(AbilitySpec->Ability);

					if (AbilityCDO->GetActivationPolicy() == EPMAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 어빌리티 실행
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// 입력 Release 관련
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

FActiveGameplayEffectHandle UPMAbilitySystemComponent::ApplyEffectToTargetWithSetByCaller(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser, TMap<FGameplayTag, float> SetbyCallerMap, float Level/* = 0*/)
{
	IAbilitySystemInterface* TargetASCInterface = Cast<IAbilitySystemInterface>(Target);
	if (TargetASCInterface == nullptr)
	{
		MCHAE_WARNING("Target actor is not inheritanced IAbilitySystemInterface.");
		return FActiveGameplayEffectHandle{};
	}

	UAbilitySystemComponent* TargetASC = TargetASCInterface->GetAbilitySystemComponent();
	if (TargetASC == nullptr)
	{
		MCHAE_WARNING("AbilitySystemComponent is not valid. Check Target's abilitysystemcomponent");
		return FActiveGameplayEffectHandle{};
	}

	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddInstigator(GetOwner(), EffectCauser);

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
	if (Spec == nullptr)
	{
		MCHAE_WARNING("EffectSpec is not valid");
		return FActiveGameplayEffectHandle{};
	}

	for (const auto& Iter : SetbyCallerMap)
	{
		Spec->SetSetByCallerMagnitude(Iter.Key, Iter.Value);
	}

	return TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
}

FActiveGameplayEffectHandle UPMAbilitySystemComponent::ApplyEffectToSelfWithSetByCaller(TSubclassOf<UGameplayEffect> EffectClass, AActor* EffectCauser, TMap<FGameplayTag, float> SetbyCallerMap, float Level /*= 0*/)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddInstigator(GetOwner(), EffectCauser);

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
	if (Spec == nullptr)
	{
		MCHAE_WARNING("EffectSpec is not valid");
		return FActiveGameplayEffectHandle{};
	}

	for (const auto& Iter : SetbyCallerMap)
	{
		Spec->SetSetByCallerMagnitude(Iter.Key, Iter.Value);
	}

	return ApplyGameplayEffectSpecToSelf(*Spec);
}
