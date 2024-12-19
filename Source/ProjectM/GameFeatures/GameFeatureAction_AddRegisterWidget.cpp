#include "GameFeatureAction_AddRegisterWidget.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Pawn.h"
#include "UI/MViewportClient.h"

void UGameFeatureAction_AddRegisterWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (GameInstance && World && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleWidgetRegistry, ChangeContext);

			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
			ActiveData.ComponentRequests = ExtensionRequestHandle;
			ActiveData.GameInstance = GameInstance;
		}
	}
}
UE_DISABLE_OPTIMIZATION
void UGameFeatureAction_AddRegisterWidget::HandleWidgetRegistry(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	if (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveWidgetRegister(ChangeContext);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
	{
		AddWidgetRegister(ChangeContext);
	}
}

void UGameFeatureAction_AddRegisterWidget::AddWidgetRegister(FGameFeatureStateChangeContext ChangeContext)
{
	const FPerContextData& ActiveData = ContextData.FindRef(ChangeContext);
	UGameInstance* GameInstance = ActiveData.GameInstance;
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;

	if (ViewportClient)
	{
		for (FMWidgetRegisterHandle RegisterHandle : WidgetRegisterHandles)
		{
			UMWidgetRegister* WidgetRegister = RegisterHandle.WidgetRegister;
			if (IsValid(WidgetRegister) && WidgetRegister->RegisterTag.IsValid())
			{
				ViewportClient->AddWidgetRegister(RegisterHandle.WidgetRegister->RegisterTag, WidgetRegister);
			}
		}
	}
}

void UGameFeatureAction_AddRegisterWidget::RemoveWidgetRegister(FGameFeatureStateChangeContext ChangeContext)
{
	const FPerContextData& ActiveData = ContextData.FindRef(ChangeContext);
	UGameInstance* GameInstance = ActiveData.GameInstance;
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;

	if (!ViewportClient)
	{
		MCHAE_WARNING("ViewportClient is null!!!! check GameInstance");
		return;
	}

	for (FMWidgetRegisterHandle RegisterHandle : WidgetRegisterHandles)
	{
		UMWidgetRegister* WidgetRegister = RegisterHandle.WidgetRegister;
		if (IsValid(WidgetRegister) && WidgetRegister->RegisterTag.IsValid())
		{
			ViewportClient->RemoveWidgetRegister(RegisterHandle.WidgetRegister->RegisterTag);
		}
	}
}
UE_ENABLE_OPTIMIZATION