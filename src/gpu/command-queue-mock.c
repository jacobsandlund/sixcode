#include "gpu/command-queue.h"
#include <stdlib.h>

struct GpuCommandQueue {
    GpuDevice *device;
};

GpuCommandQueue *gpu_command_queue_create(GpuDevice *device)
{
    GpuCommandQueue *queue = malloc(sizeof *queue);
    queue->device = device;
    return queue;
}

void gpu_command_queue_destroy(GpuCommandQueue *queue)
{
    free(queue);
}
