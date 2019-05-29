#include "resource.h"
#include "resource-manager.h"

static void resource_noop_init(Resource *resource, uintptr_t options)
{
	(void) options;
	resource->pointer = NULL;
}

static void resource_noop_destroy(Resource *resource)
{
	(void) resource;
}

const ResourceInitFn ResourceDefaultInitFn = resource_noop_init;
const ResourceDestroyFn ResourceDefaultDestroyFn = resource_noop_destroy;

void resource_register_loaders(void)
{
	ResourceLoader list_loader = {
		.type = ResourceTypeList,
		.init = ResourceDefaultInitFn,
		.destroy = ResourceDefaultDestroyFn,
	};

	resource_manager_register_loader(&list_loader);
}
