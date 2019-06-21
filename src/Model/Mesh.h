#ifndef _Mesh_h
#define _Mesh_h

#include "Spacetime.h"

typedef struct {
    float x;
    float y;
    i8 hx;
    i8 hy;
} FillMeshVertex;

typedef struct {
    float2 positionOffset;
    int2 gridPositionOffset;
} InstanceMeshVertex;

i64 FillMeshVerticesHexLength(i64 size);
void FillMeshVerticesHexSet(void *contents, uintptr_t content_init_options);
void FillMeshIndicesHexSet(void *contents, uintptr_t content_init_options);
void FillMeshVerticesRectSet(void *contents, uintptr_t content_init_options);
void FillMeshIndicesRectSet(void *contents, uintptr_t content_init_options);

#endif // _Mesh_h
