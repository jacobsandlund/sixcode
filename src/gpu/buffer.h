#ifndef GpuBuffer_h
#define GpuBuffer_h

#include "spacetime.h"
#include "gpu/device.h"

typedef enum {
   GpuBufferStorageModeTypeShared,
} GpuBufferStorageModeType;

typedef struct GpuBuffer GpuBuffer;

GpuBuffer *gpu_buffer_create_with_length(GpuDevice *device, i64 length, GpuBufferStorageModeType storage_mode);
void gpu_buffer_destroy(GpuBuffer *buffer);
void *gpu_buffer_contents(GpuBuffer *buffer);

#endif // GpuBuffer_h
