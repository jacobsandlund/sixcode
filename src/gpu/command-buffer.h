#ifndef GpuCommandBuffer_h
#define GpuCommandBuffer_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/view.h"
#include "gpu/command-queue.h"

typedef struct GpuCommandBuffer GpuCommandBuffer;

GpuCommandBuffer *gpu_command_buffer_create(GpuCommandQueue *queue);
void gpu_command_buffer_destroy(GpuCommandBuffer *buffer);
void gpu_command_buffer_present_drawable(GpuCommandBuffer *buffer, GpuView *view);
void gpu_command_buffer_commit(GpuCommandBuffer *buffer);

#endif // GpuCommandBuffer_h
