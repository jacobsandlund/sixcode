#ifndef _GpuFunction_h
#define _GpuFunction_h

#include "Gpu/Device.h"
#include "Spacetime.h"

typedef struct GpuFunction GpuFunction;

GpuFunction *GpuFunctionCreateWithName(GpuDevice *device, const char *name);
void GpuFunctionDestroy(GpuFunction *function);

#endif  // _GpuFunction_h
