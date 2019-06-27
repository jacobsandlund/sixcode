#include "Render/Layout.h"

void RenderLayoutSetType(RenderLayout *layout, RenderLayoutType type)
{
    layout->type = type;
    layout->scale = (double2){
        1.7320508075688772,  // sqrt(3)
        -1.5,                // -3.0 / 2.0
    };
}
