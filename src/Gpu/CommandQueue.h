#ifndef _GpuCommandQueue_h
#define _GpuCommandQueue_h

#include "Spacetime.h"
#include "Gpu/Device.h"

typedef struct GpuCommandQueue GpuCommandQueue;

GpuCommandQueue *GpuCommandQueueCreate(GpuDevice *device);
void GpuCommandQueueDestroy(GpuCommandQueue *queue);

#endif // _GpuCommandQueue_h
