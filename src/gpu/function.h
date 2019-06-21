#ifndef _GpuFunction_h
#define _GpuFunction_h

#include "spacetime.h"
#include "gpu/device.h"

typedef struct GpuFunction GpuFunction;

GpuFunction *gpu_function_create_with_name(GpuDevice *device, const char *name);
void gpu_function_destroy(GpuFunction *function);

#endif // _GpuFunction_h
