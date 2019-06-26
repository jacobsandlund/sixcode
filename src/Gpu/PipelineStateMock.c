#include "Gpu/PipelineState.h"

#include <stdlib.h>

struct GpuPipelineState {
    char *label;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    i64 pixel_format;
};

GpuPipelineState *GpuPipelineStateCreate(GpuDevice *device,
                                         GpuPipelineStateConfig *config)
{
    GpuPipelineState *state = malloc(sizeof *state);
    state->label = config->label;
    state->vertex_function = config->vertex_function;
    state->fragment_function = config->fragment_function;
    state->pixel_format = config->pixel_format;
    return state;
}

void GpuPipelineStateDestroy(GpuPipelineState *state)
{
    free(state);
}
