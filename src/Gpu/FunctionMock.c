#include "Gpu/Function.h"

#include <stdlib.h>

struct GpuFunction {
    char *name;
};

GpuFunction *gpu_function_create_with_name(GpuDevice *device, const char *name)
{
    GpuFunction *function = malloc(sizeof *function);
    function->name = (char *)name;
    return function;
}

void gpu_function_destroy(GpuFunction *function)
{
    free(function);
}
