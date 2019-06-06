#include <math.h>
#include <stdlib.h>
#include "model/mesh.h"
#include "render/layout.h"

#define FillMeshVerticesPerHex 6
#define FillMeshIndicesPerHex 12
#define FillMeshVerticesPerRect 4
#define FillMeshIndicesPerRect 6
#define FillMeshFraction 0.95

static float2 mesh_hex_corner(i64 corner)
{
    double angle = -M_PI / 3.0 * (0.5 + corner);
    RenderLayout l;
    render_layout_type(&l, RenderLayoutTypeHex);

    return (float2) {
        cos(angle) * FillMeshFraction / l.scale.x,
        sin(angle) * FillMeshFraction / l.scale.y,
    };
}

i64 fill_mesh_vertices_hex_length(i64 size)
{
    return size * size * FillMeshVerticesPerHex * sizeof(FillMeshVertex);
}

void fill_mesh_vertices_hex_set(void *contents, uintptr_t content_init_options)
{
    FillMeshVertex *vertices = (FillMeshVertex *) contents;
    i64 size = (i64) content_init_options;

    float2 corners[] = {
        mesh_hex_corner(0),
        mesh_hex_corner(1),
        mesh_hex_corner(2),
        mesh_hex_corner(3),
        mesh_hex_corner(4),
        mesh_hex_corner(5),
    };

    int2 h;
    i64 vi = 0;

    for (h.y = 0; h.y < size; h.y++) {
        for (h.x = 0; h.x < size; h.x++) {
            float2 center = float2_from_int2(h);
            center.x += 0.5 * (h.y & 1);

            for (i64 i = 0; i < FillMeshVerticesPerHex; i++) {
                FillMeshVertex *vx = &vertices[vi + i];
                vx->x = corners[i].x + center.x;
                vx->y = corners[i].y + center.y;
                vx->hx = h.x;
                vx->hy = h.y;
            }

            vi += FillMeshVerticesPerHex;
        }
    }
}

void fill_mesh_indices_hex_set(void *contents, uintptr_t content_init_options)
{
    u16 *indices = (u16 *) contents;
    i64 size = (i64) content_init_options;

    u16 indices_single[] = {
        1, 2, 0,
        0, 2, 3,
        0, 3, 5,
        3, 4, 5,
    };

    int2 h;
    i64 vi = 0;
    i64 ii = 0;

    for (h.y = 0; h.y < size; h.y++) {
        for (h.x = 0; h.x < size; h.x++) {
            for (i64 i = 0; i < FillMeshIndicesPerHex; i++) {
                indices[ii + i] = (u16) (vi + indices_single[i]);
            }

            ii += FillMeshIndicesPerHex;
            vi += FillMeshVerticesPerHex;
        }
    }
}

void fill_mesh_vertices_rect_set(void *contents, uintptr_t content_init_options)
{
    FillMeshVertex *vertices = (FillMeshVertex *) contents;
    i64 size = (i64) content_init_options;

    float2 rect_size = {
        0.5 * FillMeshFraction,
        0.5 * FillMeshFraction,
    };

    float2 corners[] = {
        {rect_size.x, -rect_size.y},
        {-rect_size.x, -rect_size.y},
        {-rect_size.x, rect_size.y},
        {rect_size.x, rect_size.y},
    };


    int2 h;
    i64 vi = 0;

    for (h.y = 0; h.y < size; h.y++) {
        for (h.x = 0; h.x < size; h.x++) {
            float2 center = float2_from_int2(h);

            for (i64 i = 0; i < FillMeshVerticesPerRect; i++) {
                FillMeshVertex *vx = &vertices[vi + i];
                vx->x = corners[i].x + center.x;
                vx->y = corners[i].y + center.y;
                vx->hx = h.x;
                vx->hy = h.y;
            }

            vi += FillMeshVerticesPerRect;
        }
    }
}

void fill_mesh_indices_rect_set(void *contents, uintptr_t content_init_options)
{
    u16 *indices = (u16 *) contents;
    i64 size = (i64) content_init_options;

    u16 indices_single[] = {
        0, 1, 2,
        0, 2, 3,
    };

    int2 h;
    i64 vi = 0;
    i64 ii = 0;

    for (h.y = 0; h.y < size; h.y++) {
        for (h.x = 0; h.x < size; h.x++) {
            for (i64 i = 0; i < FillMeshIndicesPerRect; i++) {
                indices[ii + i] = vi + indices_single[i];
            }

            ii += FillMeshIndicesPerRect;
            vi += FillMeshVerticesPerRect;
        }
    }
}
