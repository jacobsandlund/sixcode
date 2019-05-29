#include <assert.h>
#include <stdlib.h>
#include "resource-manager.h"

ResourceManager gResourceManager;

void resource_manager_start_up(i32 resource_capacity, i32 descriptor_capacity)
{
	sid_hash_table_init(&gResourceManager.resource_descriptors, descriptor_capacity, sizeof(ResourceDescriptor));
	sid_hash_table_init(&gResourceManager.resources, resource_capacity, sizeof(Resource));
	sid_hash_table_init(&gResourceManager.top_level_resource_ids, descriptor_capacity, 0);
}

void resource_manager_shut_down(void)
{
	sid_hash_table_destroy(&gResourceManager.resources);
	sid_hash_table_destroy(&gResourceManager.resource_descriptors);
}

void resource_manager_load_descriptors(const ResourceDescriptor *descriptors, i64 descriptors_length)
{
	SidHashTable *descriptors_table = &gResourceManager.resource_descriptors;
	assert(descriptors_table->count + descriptors_length <= descriptors_table->size);

	for (i64 i = 0; i < descriptors_length; i++) {
		sid_hash_table_insert(descriptors_table,
				descriptors[i].id, &descriptors[i]);
	}
}

void resource_manager_register_loader(ResourceLoader *loader)
{
	gResourceManager.resource_loaders[loader->type] = *loader;
}

Resource *resource_manager_load(sid resource_id)
{
	SidHashTable *resources = &gResourceManager.resources;
	Resource *resource_p = sid_hash_table_get(resources, resource_id);
	if (resource_p) {
		return resource_p;
	}

	SidHashTable *descriptors = &gResourceManager.resource_descriptors;
	ResourceDescriptor *descriptor = sid_hash_table_get(descriptors, resource_id);

	if (!descriptor) {
		u8 *string = StringIdString(resource_id);
		Log("No descriptor found for resource: '%s' (resource_id: %"
				PRIu64 ")", string, resource_id);
		abort();
	}

	Resource resource = {.id = resource_id};

	for (i32f i = 0; i < ResourceMaxNumDependencies; i++) {
		sid dependency_id = descriptor->dependencies[i];
		if (dependency_id) {
			resource.dependencies[i] = resource_manager_load(dependency_id);
		}
	}

	ResourceLoader *loader = &gResourceManager.resource_loaders[descriptor->type];
	loader->init(&resource, descriptor->options);
	sid_hash_table_insert(resources, resource_id, &resource);

	return &resource;
}

Resource *resource_manager_get(sid resource_id)
{
	Resource *resource_p = sid_hash_table_get(&gResourceManager.resources, resource_id);

	if (!resource_p) {
		u8 *string = StringIdString(resource_id);
		Log("Trying to get unloaded resource : '%s' (resource_id: %"
				PRIu64 ")", string, resource_id);
		abort();
	}

	return resource_p;
}
