#include "render/layout.h"

void RenderLayoutSetType(RenderLayout *rl, RenderLayoutType type)
{
    rl->type = type;
    rl->scale = (double2) {
        1.7320508075688772,    // sqrt(3)
        -1.5,            // -3.0 / 2.0
    };
}
