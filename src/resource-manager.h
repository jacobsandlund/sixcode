#ifndef ResourceManager_h
#define ResourceManager_h

#include "spacetime.h"
#include "resource.h"

typedef struct {
	ResourceLoader resource_loaders[ResourceTypeNumTypes];
	i64 resources_capacity;
	i64 resources_length;
	char **resource_ids;
	Resource *resources;
} ResourceManager;

void resource_manager_init(ResourceManager *rm, i64 capacity);
void resource_manager_destroy(ResourceManager *rm);

void resource_manager_load_descriptors(ResourceManager *rm, const ResourceDescriptor *descriptors, i64 descriptors_length);
void resource_manager_register_loader(ResourceManager *rm, ResourceLoader *loader);

void *resource_manager_load(ResourceManager *rm, const char *resource_id);
void *resource_manager_get(ResourceManager *rm, const char *resource_id);

#endif // ResourceManager_h
