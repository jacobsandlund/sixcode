#include "Camera/Manager.h"

CameraManager gCameraManager;

void CameraManagerInit(CameraManagerConfig *config)
{
    gCameraManager.camera.position = config->position;
}

void CameraManagerDestroy(void)
{
    // Nothing to do
}
