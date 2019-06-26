#ifndef _GpuCommandBuffer_h
#define _GpuCommandBuffer_h

#include "Gpu/CommandQueue.h"
#include "Gpu/Device.h"
#include "Gpu/View.h"
#include "Spacetime.h"

typedef struct GpuCommandBuffer GpuCommandBuffer;

GpuCommandBuffer *GpuCommandBufferCreate(GpuCommandQueue *queue);
void GpuCommandBufferDestroy(GpuCommandBuffer *buffer);
void GpuCommandBufferPresentDrawable(GpuCommandBuffer *buffer, GpuView *view);
void GpuCommandBufferCommit(GpuCommandBuffer *buffer);

#endif  // _GpuCommandBuffer_h
