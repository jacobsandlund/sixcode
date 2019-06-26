#include "Model/Mesh.c"

#include "Render/Layout.c"

#include "Test.h"

Test(FillMeshVerticesHexSet)
{
    i64 size = 32;
    i64 byte_length = FillMeshVerticesHexLength(size);
    _d(byte_length);
    //=> 73728

    FillMeshVertex *vertices = tmalloc(byte_length);
    FillMeshVerticesHexSet((void *)vertices, (uintptr_t)size);

    i64 length = byte_length / sizeof(FillMeshVertex);

    _gg(vertices[0].x, vertices[0].y);
    //=> 0.475, 0.316667
    _gg(vertices[1].x, vertices[1].y);
    //=> 3.35849e-17, 0.633333
    _gg(vertices[2].x, vertices[2].y);
    //=> -0.475, 0.316667
    _gg(vertices[3].x, vertices[3].y);
    //=> -0.475, -0.316667
    _gg(vertices[4].x, vertices[4].y);
    //=> -1.00755e-16, -0.633333
    _gg(vertices[5].x, vertices[5].y);
    //=> 0.475, -0.316667
    _gg(vertices[length - 1].x, vertices[length - 1].y);
    //=> 31.975, 30.6833

    _dd(vertices[0].hx, vertices[0].hy);
    //=> 0, 0
    _dd(vertices[5].hx, vertices[5].hy);
    //=> 0, 0
    _dd(vertices[6].hx, vertices[6].hy);
    //=> 1, 0
    _dd(vertices[length - 1].hx, vertices[length - 1].hy);
    //=> 31, 31
}

Test(FillMeshIndicesHexSet)
{
    i64 size = 32;
    i64 length = size * size * FillMeshIndicesPerHex;
    u16 *indices = tmalloc(length * sizeof *indices);
    FillMeshIndicesHexSet((void *)indices, (uintptr_t)size);

    _ddd(indices[0], indices[1], indices[2]);
    //=> 1, 2, 0
    _ddd(indices[3], indices[4], indices[5]);
    //=> 0, 2, 3
    _ddd(indices[6], indices[7], indices[8]);
    //=> 0, 3, 5
    _ddd(indices[9], indices[10], indices[11]);
    //=> 3, 4, 5
    _ddd(indices[length - 3], indices[length - 2], indices[length - 1]);
    //=> 6141, 6142, 6143
}

Test(FillMeshVerticesRectSet)
{
    i64 size = 32;
    i64 length = size * size * FillMeshVerticesPerRect;
    FillMeshVertex *vertices = tmalloc(length * sizeof *vertices);
    FillMeshVerticesRectSet((void *)vertices, (uintptr_t)size);

    _gg(vertices[0].x, vertices[0].y);
    //=> 0.475, -0.475
    _gg(vertices[1].x, vertices[1].y);
    //=> -0.475, -0.475
    _gg(vertices[2].x, vertices[2].y);
    //=> -0.475, 0.475
    _gg(vertices[3].x, vertices[3].y);
    //=> 0.475, 0.475
    _gg(vertices[length - 1].x, vertices[length - 1].y);
    //=> 31.475, 31.475

    _dd(vertices[0].hx, vertices[0].hy);
    //=> 0, 0
    _dd(vertices[3].hx, vertices[3].hy);
    //=> 0, 0
    _dd(vertices[4].hx, vertices[4].hy);
    //=> 1, 0
    _dd(vertices[length - 1].hx, vertices[length - 1].hy);
    //=> 31, 31
}

Test(FillMeshIndicesRectSet)
{
    i64 size = 32;
    i64 length = size * size * FillMeshIndicesPerRect;
    u16 *indices = tmalloc(length * sizeof *indices);
    FillMeshIndicesRectSet((void *)indices, (uintptr_t)size);

    _ddd(indices[0], indices[1], indices[2]);
    //=> 0, 1, 2
    _ddd(indices[3], indices[4], indices[5]);
    //=> 0, 2, 3
    _ddd(indices[length - 3], indices[length - 2], indices[length - 1]);
    //=> 4092, 4094, 4095
}
