#include "camera/manager.c"
#include "camera/camera.c"
#include "render/vector.c"
#include "world/vector.c"
#include "test/test.h"

Test(camera_manager_init)
{
    CameraManagerConfig config = {
        .position = {300,400,20},
    };
    camera_manager_init(&config);

    _f3(gCameraManager.camera.position);
    //=> 300, 400, 20

    camera_manager_destroy();
}
