#include "resource.h"
#include "string/manager.h"

static void resource_noop_init(Resource *resource, uintptr_t options)
{
    (void) options;
    resource->pointer = NULL;
}

static void resource_noop_destroy(Resource *resource)
{
    resource->pointer = NULL;
}

const ResourceInitFn ResourceDefaultInitFn = resource_noop_init;
const ResourceDestroyFn ResourceDefaultDestroyFn = resource_noop_destroy;
