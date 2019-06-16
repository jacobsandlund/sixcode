#ifndef GpuCommandBuffer_h
#define GpuCommandBuffer_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/command-queue.h"

typedef struct GpuCommandBuffer GpuCommandBuffer;

GpuCommandBuffer *gpu_command_buffer_create(GpuDevice *device, GpuCommandQueue *queue);
void gpu_command_buffer_destroy(GpuCommandBuffer *buffer);
void gpu_command_buffer_commit(GpuCommandBuffer *buffer, GpuCommandQueue *queue);

#endif // GpuCommandBuffer_h
