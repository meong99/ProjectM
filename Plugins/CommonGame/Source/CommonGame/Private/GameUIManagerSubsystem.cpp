// Fill out your copyright notice in the Description page of Project Settings.

#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

UGameUIManagerSubsystem::UGameUIManagerSubsystem()
{
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// CurrentPlicy가 설정되어 있지 않고, DefaultUIPolicyClass가 제대로 설정되어 있을 경우
	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		// UIPolicyClass는 BP에셋이기에, 로딩해야 함
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();

		// UIPlicyClass를 통해 NewObject로 인스텅싱해서 CurrentPolicy에 설정
		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// DedicatedServer의 경우 이 클래스를 활성화하지 않는다.
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 만약 어떠한 Subsystem도 이 클래스를 상속받지 않았다면 이 클래스는 활성화하지 않음
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}

	return false;
}

void UGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}
