#include "Camera/Camera.c"

#include "Render/Layout.c"
#include "Render/Vector.c"
#include "Render/Viewport.h"
#include "World/Vector.c"

#include "Test.h"

Test(CameraTranslate)
{
    Camera c = {
        .position = { 500, -2000, 20.0 },
    };
    RenderViewport vp = {
        .size = { 1000, 600 },
    };
    RenderLayoutSetType(&vp.layout, RenderLayoutTypeHex);
    float2 delta = { 300, 400 };

    CameraTranslate(&c, &vp, delta);

    _f3(c.position);
    //=> 508.66, -1986.67, 20
}

Test(CameraZoomAtScreenVector)
{
    Camera c = {
        .position = { 500, -2000, 20.0 },
    };
    RenderViewport vp = {
        .size = { 1000, 600 },
    };
    RenderLayoutSetType(&vp.layout, RenderLayoutTypeHex);

    float2 v = vp.size;
    CameraZoomAtScreenVector(&c, &vp, v, 30.0);
    _f3(c.position);
    //=> 504.811, -1996.67, 30

    v = (float2){ 300, 400 };
    CameraZoomAtScreenVector(&c, &vp, v, 10.0);

    _f3(c.position);
    //=> 512.509, -2001.11, 10
}
