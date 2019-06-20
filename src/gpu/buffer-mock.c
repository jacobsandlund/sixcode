#include "gpu/buffer.h"
#include <stdlib.h>

struct GpuBuffer {
    GpuDevice *device;
    GpuBufferStorageModeType storage_mode;
    void *bytes;
    i64 length;
};

GpuBuffer *gpu_buffer_create_with_length(GpuDevice *device, i64 length, GpuBufferStorageModeType storage_mode)
{
    GpuBuffer *buffer = malloc(sizeof *buffer);

    buffer->device = device;
    buffer->storage_mode = storage_mode;
    buffer->bytes = malloc(length);
    buffer->length = length;

    return buffer;
}

void gpu_buffer_destroy(GpuBuffer *buffer)
{
    free(buffer->bytes);
    free(buffer);
}

void *gpu_buffer_contents(GpuBuffer *buffer)
{
    return buffer->bytes;
}
