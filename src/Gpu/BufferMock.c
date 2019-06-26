#include "Gpu/Buffer.h"

#include <stdlib.h>

struct GpuBuffer {
    GpuDevice *device;
    GpuBufferStorageMode storage_mode;
    void *bytes;
    i64 length;
};

GpuBuffer *GpuBufferCreateWithLength(GpuDevice *device,
                                     i64 length,
                                     GpuBufferStorageMode storage_mode)
{
    GpuBuffer *buffer = malloc(sizeof *buffer);

    buffer->device = device;
    buffer->storage_mode = storage_mode;
    buffer->bytes = malloc(length);
    buffer->length = length;

    return buffer;
}

void GpuBufferDestroy(GpuBuffer *buffer)
{
    free(buffer->bytes);
    free(buffer);
}

void *GpuBufferContents(GpuBuffer *buffer)
{
    return buffer->bytes;
}
