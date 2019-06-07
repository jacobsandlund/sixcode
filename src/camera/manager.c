#include "camera/manager.h"

CameraManager gCameraManager;

void camera_manager_init(CameraManagerConfig *config)
{
    gCameraManager.camera.position = config->position;
}

void camera_manager_destroy(void)
{
    // Nothing to do
}
