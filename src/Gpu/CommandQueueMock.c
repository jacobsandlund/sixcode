#include "Gpu/CommandQueue.h"
#include <stdlib.h>

struct GpuCommandQueue {
    GpuDevice *device;
};

GpuCommandQueue *GpuCommandQueueCreate(GpuDevice *device)
{
    GpuCommandQueue *queue = malloc(sizeof *queue);
    queue->device = device;
    return queue;
}

void GpuCommandQueueDestroy(GpuCommandQueue *queue)
{
    free(queue);
}
