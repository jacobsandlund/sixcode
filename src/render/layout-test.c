#include "render/layout.c"
#include "test/test.h"

Test(render_layout_type)
{
    RenderLayout l;
    render_layout_type(&l, RenderLayoutTypeHex);
    _d(l.type == RenderLayoutTypeHex);
    //=> 1

    _f2(l.scale);
    //=> 1.73205, -1.5

    render_layout_type(&l, RenderLayoutTypeRect);
    _d(l.type == RenderLayoutTypeRect);
    //=> 1

    _f2(l.scale);
    //=> 1.73205, -1.5
}
