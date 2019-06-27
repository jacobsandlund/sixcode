#include "Gpu/RenderPassConfig.h"

#include <stdlib.h>

void GpuRenderPassConfigDestroy(GpuRenderPassConfig *config)
{
    free(config);
}
