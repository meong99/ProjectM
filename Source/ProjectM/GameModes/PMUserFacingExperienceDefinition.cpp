#include "PMUserFacingExperienceDefinition.h"
#include "CommonSessionSubSystem.h"
#include "Engine/AssetManager.h"
#include "AssetRegistry/AssetData.h"

UPMUserFacingExperienceDefinition::UPMUserFacingExperienceDefinition()
{

}

UCommonSession_HostSessionRequest* UPMUserFacingExperienceDefinition::CreateHostingRequst(const FString& Ip) const
{
    const FString ExperienceName = ExperienceId.PrimaryAssetName.ToString();

    // NewObject를 생성해서 바로 사용하지 않으면 GC로 인해 Dangling이 날 수 있다.
    UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	if (!Ip.IsEmpty())
	{
		Result->URL = Ip;
	}
	else
	{
		Result->URL= GetMapName();
	}
    Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

    return Result;
}

FString UPMUserFacingExperienceDefinition::GetMapName() const
{
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapId, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}

	return FString();
}
