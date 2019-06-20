#ifndef GpuPipelineState_h
#define GpuPipelineState_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/function.h"

typedef struct {
    char *label;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    i64 pixel_format;
} GpuPipelineStateConfig;

typedef struct GpuPipelineState GpuPipelineState;

GpuPipelineState *gpu_pipeline_state_create(GpuDevice *device, GpuPipelineStateConfig *config);
void gpu_pipeline_state_destroy(GpuPipelineState *state);

#endif // GpuPipelineState_h
