#include "MWidgetRegister.h"
#include "Blueprint/UserWidget.h"
#include "../PMActivatableWidget.h"

UMWidgetRegister::UMWidgetRegister()
{
}

void UMWidgetRegister::SetWorld(const UObject* InWorldContext)
{
	WorldContext = InWorldContext;
	check(WorldContext);
}

void UMWidgetRegister::AsyncLoadAllWidgets()
{
	for (const auto Iter : MappedWidgetData.WidgetData)
	{
		const TSoftClassPtr<UUserWidget>& WidgetSoftClass = Iter.Value;
		AsyncLoadWidget_Impl(Iter.Key, WidgetSoftClass);
	}
}

TSubclassOf<UUserWidget> UMWidgetRegister::GetWidgetClass(const FGameplayTag& Tag)
{
	const TSoftClassPtr<UUserWidget>& WidgetSoftClass = MappedWidgetData.WidgetData.FindRef(Tag);
	TSubclassOf<UUserWidget> WidgetClass = WidgetSoftClass.Get();
	if (!WidgetClass)
	{
		WidgetClass = WidgetSoftClass.LoadSynchronous();
	}

	return WidgetClass;
}

UUserWidget* UMWidgetRegister::GetWidgetInstanceAndLoadIfNotLoaded(const FGameplayTag& Tag)
{
	TObjectPtr<UUserWidget> CachedWidget = MappedWidgetInstances.WidgetInstances.FindRef(Tag);
	if (CachedWidget == nullptr)
	{
		return AsyncLoadWidgetByTag(Tag);
	}

	return CachedWidget;
}

UUserWidget* UMWidgetRegister::AsyncLoadWidgetByTag(const FGameplayTag& Tag)
{
	const TSoftClassPtr<UUserWidget>& WidgetSoftClass = MappedWidgetData.WidgetData.FindRef(Tag);
	return AsyncLoadWidget_Impl(Tag, WidgetSoftClass);
}

UUserWidget* UMWidgetRegister::AsyncLoadWidget_Impl(const FGameplayTag& Tag, const TSoftClassPtr<UUserWidget>& WidgetSoftClass)
{
	if (UUserWidget* WidgetInstance = MappedWidgetInstances.WidgetInstances.FindRef(Tag))
	{
		MCHAE_WARNING("This Widget Already Instanced. Check if the widget is registered twice. WidgetTag = %s", *Tag.ToString());
		return WidgetInstance;
	}

	if (WorldContext == nullptr)
	{
		MCHAE_ERROR("WorldContext is null");
		return nullptr;
	}

	if (WidgetSoftClass)
	{
		TSubclassOf<UUserWidget> WidgetClass = WidgetSoftClass.Get();
		if (!WidgetClass)
		{
			WidgetClass = WidgetSoftClass.LoadSynchronous();
		}

		if (WidgetClass)
		{
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(WorldContext->GetWorld(), WidgetClass);
#pragma TODO("이거 Activatable말고 UMG로 가자")
			if (UPMActivatableWidget* ActivatableWidget = Cast<UPMActivatableWidget>(NewWidget))
			{
				ActivatableWidget->WidgetTag = Tag;
			}

			MappedWidgetInstances.WidgetInstances.Emplace(Tag, NewWidget);

			return NewWidget;
		}
		else
		{
			MCHAE_WARNING("WidgetClass Load Fail");
		}
	}
	else
	{
		MCHAE_WARNING("Widget Class is not registered!");
	}

	return nullptr;
}
