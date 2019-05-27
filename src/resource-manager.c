#include <assert.h>
#include <stdlib.h>
#include "resource-manager.h"

void resource_manager_init(ResourceManager *rm, i64 capacity)
{
	rm->resources_capacity = capacity;
	rm->resources_length = 0;
	rm->resource_ids = malloc(capacity * sizeof *rm->resource_ids);
	rm->resources = malloc(capacity * sizeof *rm->resources);
}

void resource_manager_destroy(ResourceManager *rm)
{
	free(rm->resource_ids);
	free(rm->resources);
}

void resource_manager_load_descriptors(ResourceManager *rm, const ResourceDescriptor *descriptors, i64 descriptors_length)
{
	assert(rm->resources_length + descriptors_length <= rm->resources_capacity);
	i64 ri = rm->resources_length;

	for (i64 i = 0; i < descriptors_length; i++) {
		rm->resources[ri + i].descriptor = descriptors[i];
		rm->resource_ids[ri + i] = descriptors[i].id;
	}

	rm->resources_length += descriptors_length;
}

void resource_manager_register_loader(ResourceManager *rm, ResourceLoader *loader)
{
	rm->resource_loaders[loader->type] = *loader;
}
