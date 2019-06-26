#ifndef _GpuPipelineState_h
#define _GpuPipelineState_h

#include "gpu/device.h"
#include "gpu/function.h"
#include "spacetime.h"

typedef struct {
    char *label;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    i64 pixel_format;
} GpuPipelineStateConfig;

typedef struct GpuPipelineState GpuPipelineState;

GpuPipelineState *GpuPipelineStateCreate(GpuDevice *device,
                                         GpuPipelineStateConfig *config);
void GpuPipelineStateDestroy(GpuPipelineState *state);

#endif  // _GpuPipelineState_h
