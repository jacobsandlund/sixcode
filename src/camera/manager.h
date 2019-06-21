#ifndef _CameraManager_h
#define _CameraManager_h

#include "spacetime.h"
#include "camera/camera.h"

typedef struct {
    float3 position;
} CameraManagerConfig;

typedef struct {
    Camera camera;
} CameraManager;

extern CameraManager gCameraManager;

void CameraManagerInit(CameraManagerConfig *config);
void CameraManagerDestroy(void);

#endif // _CameraManager_h
