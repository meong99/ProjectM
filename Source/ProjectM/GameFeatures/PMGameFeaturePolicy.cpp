#include "PMGameFeaturePolicy.h"

UPMGameFeaturePolicy::UPMGameFeaturePolicy()
{

}

void UPMGameFeaturePolicy::InitGameFeatureManager()
{
	Super::InitGameFeatureManager();
}

void UPMGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();
}

void UPMGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
}

void UPMGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
}
