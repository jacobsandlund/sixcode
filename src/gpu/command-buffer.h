#ifndef _GpuCommandBuffer_h
#define _GpuCommandBuffer_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/view.h"
#include "gpu/command-queue.h"

typedef struct GpuCommandBuffer GpuCommandBuffer;

GpuCommandBuffer *GpuCommandBufferCreate(GpuCommandQueue *queue);
void GpuCommandBufferDestroy(GpuCommandBuffer *buffer);
void GpuCommandBufferPresentDrawable(GpuCommandBuffer *buffer, GpuView *view);
void GpuCommandBufferCommit(GpuCommandBuffer *buffer);

#endif // _GpuCommandBuffer_h
