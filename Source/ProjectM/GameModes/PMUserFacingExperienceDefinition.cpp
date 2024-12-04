#include "PMUserFacingExperienceDefinition.h"
#include "CommonSessionSubSystem.h"

UPMUserFacingExperienceDefinition::UPMUserFacingExperienceDefinition()
{

}

UCommonSession_HostSessionRequest* UPMUserFacingExperienceDefinition::CreateHostingRequst() const
{
    const FString ExperienceName = ExperienceId.PrimaryAssetName.ToString();

    // NewObject를 생성해서 바로 사용하지 않으면 GC로 인해 Dangling이 날 수 있다.
    UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
    Result->MapId = MapId;
    Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

    return Result;
}
