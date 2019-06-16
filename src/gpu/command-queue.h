#ifndef GpuCommandQueue_h
#define GpuCommandQueue_h

#include "spacetime.h"
#include "gpu/device.h"

typedef enum {
    GpuCommandQueueTypeRender,
    GpuCommandQueueTypeCompute,
    GpuCommandQueueTypeCopy,
};

typedef struct GpuCommandQueue GpuCommandQueue;

GpuCommandQueue *gpu_command_queue_create(GpuDevice *device);
void gpu_command_queue_destroy(GpuCommandQueue *queue);

#endif // GpuCommandQueue_h
