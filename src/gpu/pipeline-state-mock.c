#include "gpu/pipeline-state.h"
#include <stdlib.h>

struct GpuPipelineState {
    char *label;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    i64 pixel_format;
};

GpuPipelineState *gpu_pipeline_state_create(GpuDevice *device, GpuPipelineStateConfig *config)
{
    GpuPipelineState *state = malloc(sizeof *state);
    state->label = config->label;
    state->vertex_function = config->vertex_function;
    state->fragment_function = config->fragment_function;
    state->pixel_format = config->pixel_format;
    return state;
}

void gpu_pipeline_state_destroy(GpuPipelineState *state)
{
    free(state);
}
