#include "Gpu/Function.h"

#include <stdlib.h>

struct GpuFunction {
    GpuDevice *device;
    char *name;
};

GpuFunction *GpuFunctionCreateWithName(GpuDevice *device, const char *name)
{
    GpuFunction *function = malloc(sizeof *function);
    function->device = device;
    function->name = (char *)name;
    return function;
}

void GpuFunctionDestroy(GpuFunction *function)
{
    free(function);
}
