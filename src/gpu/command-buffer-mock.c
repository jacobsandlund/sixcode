#include "gpu/command-buffer.h"

struct GpuCommandBuffer {
    GpuCommandQueue *queue;
    GpuView *presented_drawable_view;
    bool committed;
};

GpuCommandBuffer *gpu_command_buffer_create(GpuCommandQueue *queue)
{
    GpuCommandBuffer *buffer = malloc(sizeof *buffer);
    buffer->queue = queue;
    buffer->presented_drawable_view = NULL;
    return buffer;
}

void gpu_command_buffer_destroy(GpuCommandBuffer *buffer)
{
    free(buffer);
}

void gpu_command_buffer_present_drawable(GpuCommandBuffer *buffer, GpuView *view)
{
    buffer->presented_drawable_view = view;
}

void gpu_command_buffer_commit(GpuCommandBuffer *buffer)
{
    buffer->committed = true;
}
