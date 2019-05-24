#include <stdlib.h>
#include "resource-manager.h"

void resource_manager_init(ResourceManager *rm, i64 capacity)
{
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
	for (i64 i = 0; i < descriptors_length; i++) {
		rm->resources[i].descriptor = descriptors[i];
		rm->resource_ids[i] = descriptors[i].id;
	}
}
