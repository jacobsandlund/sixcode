#include "Camera/Camera.h"

#include "Render/Vector.h"

void CameraTranslate(Camera *c, RenderViewport *vp, float2 delta)
{
    float2 delta_scaled = {
        (double)delta.x / vp->layout.scale.x / (double)c->position.z,
        (double)delta.y / vp->layout.scale.y / (double)c->position.z,
    };

    c->position.x += delta_scaled.x;
    c->position.y -= delta_scaled.y;
}

void CameraZoomAtScreenVector(Camera *c,
                              RenderViewport *vp,
                              float2 v,
                              float new_camera_scale)
{
    float2 world_vector_old = RenderVectorScreenToWorld(vp, &c->position, v);
    c->position = (float3){ 0.0f, 0.0f, new_camera_scale };
    float2 world_vector_untranslated =
            RenderVectorScreenToWorld(vp, &c->position, v);

    c->position.x = world_vector_old.x - world_vector_untranslated.x;
    c->position.y = world_vector_old.y - world_vector_untranslated.y;
}
