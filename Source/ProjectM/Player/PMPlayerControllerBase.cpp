#include "PMPlayerControllerBase.h"
#include "Camera/PMPlayerCameraManager.h"

APMPlayerControllerBase::APMPlayerControllerBase()
{
	PlayerCameraManagerClass = APMPlayerCameraManager::StaticClass();
}
