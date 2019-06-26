#ifndef _GpuDevice_h
#define _GpuDevice_h

#include "Spacetime.h"

typedef struct GpuDevice GpuDevice;

GpuDevice *GpuDeviceCreate(void);
void GpuDeviceDestroy(GpuDevice *device);

#endif  // _GpuDevice_h
