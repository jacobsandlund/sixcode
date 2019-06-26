#ifndef _GpuCommandQueue_h
#define _GpuCommandQueue_h

#include "Gpu/Device.h"
#include "Spacetime.h"

typedef struct GpuCommandQueue GpuCommandQueue;

GpuCommandQueue *GpuCommandQueueCreate(GpuDevice *device);
void GpuCommandQueueDestroy(GpuCommandQueue *queue);

#endif  // _GpuCommandQueue_h
