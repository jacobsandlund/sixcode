#include "Camera/Manager.c"
#include "Camera/Camera.c"
#include "Render/Vector.c"
#include "World/Vector.c"
#include "Test.h"

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
