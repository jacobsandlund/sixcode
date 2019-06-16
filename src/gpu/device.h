#ifndef GpuDevice_h
#define GpuDevice_h

#include "spacetime.h"
#include "gpu/command-queue.h"

typedef struct GpuDevice GpuDevice;

GpuDevice *gpu_device_create(void);
void gpu_device_destroy(GpuDevice *device);

#endif // GpuDevice_h
