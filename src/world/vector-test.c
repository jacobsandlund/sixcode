#include "test.h"
#include "world/vector.c"
#include "render/layout.c"

Test(world_vector_round_hex)
{
    RenderLayout l;
    render_layout_type(&l, RenderLayoutTypeHex);

    float2 a = {0, 0};
    float2 b = {0.5, -1};
    float2 c = {-0.5, -1};
    float2 d = {-14.5, 63};
    float2 closer_to_a = {
        a.x * 0.4 + b.x * 0.3 + c.x * 0.3,
        a.y * 0.4 + b.y * 0.3 + c.y * 0.3,
    };
    float2 closer_to_b = {
        a.x * 0.3 + b.x * 0.4 + c.x * 0.3,
        a.y * 0.3 + b.y * 0.4 + c.y * 0.3,
    };
    float2 closer_to_c = {
        a.x * 0.3 + b.x * 0.3 + c.x * 0.4,
        a.y * 0.3 + b.y * 0.3 + c.y * 0.4,
    };
    float2 between_c_and_d_1 = {
        a.x * 0.501 + d.x * 0.499,
        a.y * 0.501 + d.y * 0.499,
    };
    float2 between_c_and_d_2 = {
        a.x * 0.449 + d.x * 0.501,
        a.y * 0.449 + d.y * 0.501,
    };

    _i2(world_vector_round_hex(closer_to_a));
    //=> 0, 0
    _i2(world_vector_round_hex(closer_to_b));
    //=> 0, -1
    _i2(world_vector_round_hex(closer_to_c));
    //=> -1, -1
    _i2(world_vector_round(&l, between_c_and_d_1));
    //=> -8, 31
    _i2(world_vector_round(&l, between_c_and_d_2));
    //=> -7, 32
}

Test(world_vector_round_rect)
{
    RenderLayout l;
    render_layout_type(&l, RenderLayoutTypeRect);

    float2 a = {0, 0};
    float2 b = {1, -1};
    float2 c = {0, -1};
    float2 d = {-15, 63};
    float2 closer_to_a = {
        a.x * 0.6 + b.x * 0.4,
        a.y * 0.6 + b.y * 0.4,
    };
    float2 closer_to_b = {
        a.x * 0.4 + b.x * 0.6,
        a.y * 0.4 + b.y * 0.6,
    };
    float2 closer_to_c = {
        b.x * 0.4 + c.x * 0.6,
        b.y * 0.4 + c.y * 0.6,
    };
    float2 between_c_and_d_1 = {
        a.x * 0.501 + d.x * 0.499,
        a.y * 0.501 + d.y * 0.499,
    };
    float2 between_c_and_d_2 = {
        a.x * 0.449 + d.x * 0.501,
        a.y * 0.449 + d.y * 0.501,
    };

    _i2(world_vector_round_rect(closer_to_a));
    //=> 0, 0
    _i2(world_vector_round_rect(closer_to_b));
    //=> 1, -1
    _i2(world_vector_round_rect(closer_to_c));
    //=> 0, -1
    _i2(world_vector_round(&l, between_c_and_d_1));
    //=> -7, 31
    _i2(world_vector_round(&l, between_c_and_d_2));
    //=> -8, 32
}
