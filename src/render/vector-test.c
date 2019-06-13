#include "render/vector.c"
#include "math/quad.c"
#include "render/layout.c"
#include "render/viewport.h"
#include "world/vector.c"
#include "test.h"

Test(render_vector_screen_to_world)
{
    float3 camera = {-100, -200, 10};
    RenderViewport vp = {
        .size = {1000, 600},
    };
    render_layout_type(&vp.layout, RenderLayoutTypeHex);

    float2 v1 = {125.981, 275};
    float2 v2 = {980.3, 540};

    _f2(render_vector_screen_to_world(&vp, &camera, v1));
    //=> -121.594, -201.667

    _f2(render_vector_screen_to_world(&vp, &camera, v2));
    //=> -72.2699, -184
}

Test(render_vector_viewport_to_world_quad_hex)
{
    Quad q;
    float3 camera = {1.64184, 0.75, 32.0};
    RenderViewport vp = {
        .size = {2000, 1440},
    };
    render_layout_type(&vp.layout, RenderLayoutTypeHex);

    // Wide viewport
    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (-17, -14), (20, 16)

    // Narrow with bottom and tops showing
    camera = (float3) {1.13244, -1.50198, 256.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (-2, -4), (3, 1)

    // Right and left barely showing
    camera = (float3) {1.01235, -1.98722, 1024.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (0, -3), (2, -1)

    // Almost only showing one hex
    camera = (float3) {8.54057, -3.04374, 1448.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (8, -4), (9, -3)

    // Only showing one hex
    camera = (float3) {1.00386, -2.02635, 1448.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (1, -2), (1, -2)
}

Test(render_vector_viewport_to_world_quad_rect)
{
    Quad q;
    float3 camera = {1.64184, 0.75, 32.0};
    RenderViewport vp = {
        .size = {2000, 1440},
    };
    render_layout_type(&vp.layout, RenderLayoutTypeRect);

    // Wide viewport
    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (-16, -14), (20, 16)

    // Narrow with bottom and tops showing
    camera = (float3) {1.13244, -1.50198, 256.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (-1, -3), (3, 0)

    // Right and left barely showing
    camera = (float3) {1.01235, -1.98722, 1024.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (0, -2), (2, -2)

    // Almost only showing one hex
    camera = (float3) {8.54057, -3.04374, 1448.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (8, -3), (9, -3)

    // Only showing one hex
    camera = (float3) {1.00386, -2.02635, 1448.0};

    render_vector_viewport_to_world_quad(&vp, &camera, &q);
    _qd(q);
    //=> (1, -2), (1, -2)
}
