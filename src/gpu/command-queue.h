#ifndef _GpuCommandQueue_h
#define _GpuCommandQueue_h

#include "spacetime.h"
#include "gpu/device.h"

typedef struct GpuCommandQueue GpuCommandQueue;

GpuCommandQueue *GpuCommandQueueCreate(GpuDevice *device);
void GpuCommandQueueDestroy(GpuCommandQueue *queue);

#endif // _GpuCommandQueue_h
