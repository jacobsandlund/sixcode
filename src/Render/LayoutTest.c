#include "Render/Layout.c"
#include "Test.h"

Test(RenderLayoutSetType)
{
    RenderLayout l;
    RenderLayoutSetType(&l, RenderLayoutTypeHex);
    _d(l.type == RenderLayoutTypeHex);
    //=> 1

    _f2(l.scale);
    //=> 1.73205, -1.5

    RenderLayoutSetType(&l, RenderLayoutTypeRect);
    _d(l.type == RenderLayoutTypeRect);
    //=> 1

    _f2(l.scale);
    //=> 1.73205, -1.5
}
