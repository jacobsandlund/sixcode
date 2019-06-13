#include "render/view-matrix.c"
#include "render/layout.c"
#include "render/viewport.h"
#include "test.h"

Test(render_view_matrix_init)
{
    float4x4 vm;
    render_view_matrix_init(&vm);
    _gggg(vm.columns[0][0], vm.columns[1][0], vm.columns[2][2], vm.columns[3][3]);
    //=> 0, 0, 0, 0
}

Test(render_view_matrix_update)
{
    float4x4 vm;
    render_view_matrix_init(&vm);

    float3 camera = {100, -200, 10.0};
    RenderViewport viewport = {
        .size = {1000, 600},
    };
    render_layout_type(&viewport.layout, RenderLayoutTypeHex);
    float2 offset = {93.0f, -13.0f};

    render_view_matrix_update(&vm, &viewport, &camera, offset);

    _gggg(vm.columns[0][0], vm.columns[0][1], vm.columns[0][2], vm.columns[0][3]);
    //=> 0.00173205, 0, 0, 0
    _gggg(vm.columns[1][0], vm.columns[1][1], vm.columns[1][2], vm.columns[1][3]);
    //=> 0, -0.0025, 0, 0
    _gggg(vm.columns[2][0], vm.columns[2][1], vm.columns[2][2], vm.columns[2][3]);
    //=> 0, 0, 0, 0
    _gggg(vm.columns[3][0], vm.columns[3][1], vm.columns[3][2], vm.columns[3][3]);
    //=> -0.0121244, -0.4675, 0, 0.05
}
