#include "Render/Vector.h"
#include "World/Vector.h"
#include <math.h>
#include <stdlib.h>

#define RenderVectorHexTopPointCutoff 0.3333333333333333
#define RenderVectorHexBottomPointCutoff 0.6666666666666666

static const double RenderVectorDoubleEpsilon = 1e-9;

float2 RenderVectorScreenToWorld(RenderViewport *vp, float3 *camera, float2 v)
{
    float2 v_moved = {
         v.x - vp->size.x / 2.0f,
        -v.y + vp->size.y / 2.0f,
    };

    float2 v_scaled = {
        (double) v_moved.x / vp->layout.scale.x / (double) camera->z,
        (double) v_moved.y / vp->layout.scale.y / (double) camera->z,
    };

    return (float2) {
        v_scaled.x + camera->x,
        v_scaled.y + camera->y,
    };
}

static void RenderVectorViewportToWorldQuadHex(RenderViewport *vp, float3 *camera, Quad *out_q)
{
    float2 top_left_point = {-1, -1};
    float2 bottom_right_point = {
        vp->size.x + 1,
        vp->size.y + 1,
    };
    float2 top_left = RenderVectorScreenToWorld(vp, camera, top_left_point);
    float2 bottom_right = RenderVectorScreenToWorld(vp, camera, bottom_right_point);

    i64 top = floor(top_left.y);
    i64 double_left = floor(2.0 * top_left.x);
    i64 bottom = floor(bottom_right.y);
    i64 double_right = ceil(2.0 * (double) bottom_right.x + RenderVectorDoubleEpsilon);

    top += top_left.y - top > RenderVectorHexBottomPointCutoff;
    bottom += bottom_right.y - bottom > RenderVectorHexTopPointCutoff;

    out_q->min = (int2) {(i32) (double_left >> 1), (i32) top};
    out_q->max = (int2) {(i32) (double_right >> 1), (i32) bottom};

    if (double_right - double_left <= 2 || bottom - top <= 2) {
        float2 top_right = {bottom_right.x, top_left.y};
        float2 bottom_left = {top_left.x, bottom_right.y};

        int2 top_left_hex = WorldVectorRoundHex(top_left);
        int2 top_right_hex = WorldVectorRoundHex(top_right);
        int2 bottom_left_hex = WorldVectorRoundHex(bottom_left);
        int2 bottom_right_hex = WorldVectorRoundHex(bottom_right);

        if (top_left_hex.x == top_right_hex.x) {
            out_q->min.y = top_left_hex.y;
        }

        if (bottom_left_hex.x == bottom_right_hex.x) {
            out_q->max.y = bottom_left_hex.y;
        }

        if (top_left_hex.y == bottom_left_hex.y) {
            out_q->min.x = top_left_hex.x;
        }

        if (top_right_hex.y == bottom_right_hex.y) {
            out_q->max.x = top_right_hex.x;
        }
    }
}

static void RenderVectorViewportToWorldQuadRect(RenderViewport *vp, float3 *camera, Quad *out_q)
{
    float2 top_left_point = {-1, -1};
    float2 bottom_right_point = {
        vp->size.x + 1,
        vp->size.y + 1,
    };

    out_q->min = WorldVectorRoundRect(
            RenderVectorScreenToWorld(vp, camera, top_left_point));
    out_q->max = WorldVectorRoundRect(
            RenderVectorScreenToWorld(vp, camera, bottom_right_point));
}

void RenderVectorViewportToWorldQuad(RenderViewport *vp, float3 *camera, Quad *out_q)
{
    switch (vp->layout.type) {
    case RenderLayoutTypeHex:
        RenderVectorViewportToWorldQuadHex(vp, camera, out_q);
        break;
    case RenderLayoutTypeRect:
        RenderVectorViewportToWorldQuadRect(vp, camera, out_q);
        break;
    }
}
