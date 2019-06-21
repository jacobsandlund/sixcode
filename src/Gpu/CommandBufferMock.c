#include "Gpu/CommandBuffer.h"

struct GpuCommandBuffer {
    GpuCommandQueue *queue;
    GpuView *presented_drawable_view;
    bool committed;
};

GpuCommandBuffer *GpuCommandBufferCreate(GpuCommandQueue *queue)
{
    GpuCommandBuffer *buffer = malloc(sizeof *buffer);
    buffer->queue = queue;
    buffer->presented_drawable_view = NULL;
    return buffer;
}

void GpuCommandBufferDestroy(GpuCommandBuffer *buffer)
{
    free(buffer);
}

void GpuCommandBufferPresentDrawable(GpuCommandBuffer *buffer, GpuView *view)
{
    buffer->presented_drawable_view = view;
}

void GpuCommandBufferCommit(GpuCommandBuffer *buffer)
{
    buffer->committed = true;
}
