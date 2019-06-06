#include "test.h"
#include "camera/camera.c"
#include "render/viewport.h"
#include "render/vector.c"
#include "render/layout.c"
#include "world/vector.c"

Test(camera_translate)
{
    Camera c = {
        .position = {500, -2000, 20.0},
    };
    RenderViewport vp = {
        .size = {1000, 600},
    };
    render_layout_type(&vp.layout, RenderLayoutTypeHex);
    float2 delta = {300, 400};

    camera_translate(&c, &vp, delta);

    _f3(c.position);
    //=> 508.66, -1986.67, 20
}

Test(camera_zoom_at_screen_vector)
{
    Camera c = {
        .position = {500, -2000, 20.0},
    };
    RenderViewport vp = {
        .size = {1000, 600},
    };
    render_layout_type(&vp.layout, RenderLayoutTypeHex);

    float2 v = vp.size;
    camera_zoom_at_screen_vector(&c, &vp, v, 30.0);
    _f3(c.position);
    //=> 504.811, -1996.67, 30

    v = (float2) {300, 400};
    camera_zoom_at_screen_vector(&c, &vp, v, 10.0);

    _f3(c.position);
    //=> 512.509, -2001.11, 10
}
