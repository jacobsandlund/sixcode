#include "camera/manager.c"
#include "camera/camera.c"
#include "render/vector.c"
#include "world/vector.c"
#include "test.h"

Test(CameraManagerInit)
{
    CameraManagerConfig config = {
        .position = {300,400,20},
    };
    CameraManagerInit(&config);

    _f3(gCameraManager.camera.position);
    //=> 300, 400, 20

    CameraManagerDestroy();
}
