#ifndef CameraManager_h
#define CameraManager_h

#include "spacetime.h"
#include "camera.h"

typedef struct {
    float3 position;
} CameraManagerConfig;

typedef struct {
    Camera camera;
} CameraManager;

extern CameraManager gCameraManager;

void camera_manager_init(CameraManagerConfig *config);
void camera_manager_destroy(void);

#endif // CameraManager_h
