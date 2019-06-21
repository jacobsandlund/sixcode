#include "Render/Vector.c"
#include "Math/Quad.c"
#include "Render/Layout.c"
#include "Render/Viewport.h"
#include "World/Vector.c"
#include "Test.h"

Test(RenderVectorScreenToWorld)
{
    float3 camera = {-100, -200, 10};
    RenderViewport vp = {
        .size = {1000, 600},
    };
    RenderLayoutSetType(&vp.layout, RenderLayoutTypeHex);

    float2 v1 = {125.981, 275};
    float2 v2 = {980.3, 540};

    _f2(RenderVectorScreenToWorld(&vp, &camera, v1));
    //=> -121.594, -201.667

    _f2(RenderVectorScreenToWorld(&vp, &camera, v2));
    //=> -72.2699, -184
}

Test(RenderVectorViewportToWorldQuadHex)
{
    Quad q;
    float3 camera = {1.64184, 0.75, 32.0};
    RenderViewport vp = {
        .size = {2000, 1440},
    };
    RenderLayoutSetType(&vp.layout, RenderLayoutTypeHex);

    // Wide viewport
    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (-17, -14), (20, 16)

    // Narrow with bottom and tops showing
    camera = (float3) {1.13244, -1.50198, 256.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (-2, -4), (3, 1)

    // Right and left barely showing
    camera = (float3) {1.01235, -1.98722, 1024.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (0, -3), (2, -1)

    // Almost only showing one hex
    camera = (float3) {8.54057, -3.04374, 1448.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (8, -4), (9, -3)

    // Only showing one hex
    camera = (float3) {1.00386, -2.02635, 1448.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (1, -2), (1, -2)
}

Test(RenderVectorViewportToWorldQuadRect)
{
    Quad q;
    float3 camera = {1.64184, 0.75, 32.0};
    RenderViewport vp = {
        .size = {2000, 1440},
    };
    RenderLayoutSetType(&vp.layout, RenderLayoutTypeRect);

    // Wide viewport
    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (-16, -14), (20, 16)

    // Narrow with bottom and tops showing
    camera = (float3) {1.13244, -1.50198, 256.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (-1, -3), (3, 0)

    // Right and left barely showing
    camera = (float3) {1.01235, -1.98722, 1024.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (0, -2), (2, -2)

    // Almost only showing one hex
    camera = (float3) {8.54057, -3.04374, 1448.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (8, -3), (9, -3)

    // Only showing one hex
    camera = (float3) {1.00386, -2.02635, 1448.0};

    RenderVectorViewportToWorldQuad(&vp, &camera, &q);
    _qd(q);
    //=> (1, -2), (1, -2)
}
