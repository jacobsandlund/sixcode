#ifndef ResourceManager_h
#define ResourceManager_h

#include "spacetime.h"
#include "resource.h"
#include "sid-hash-table.h"

typedef struct {
	ResourceLoader resource_loaders[ResourceTypeNumTypes];
	SidHashTable resource_descriptors;
	SidHashTable resources;
	SidHashTable top_level_resource_ids;
} ResourceManager;

extern ResourceManager gResourceManager;

void resource_manager_start_up(i32 resource_capacity, i32 descriptor_capacity);
void resource_manager_shut_down(void);

void resource_manager_load_descriptors(const ResourceDescriptor *descriptors, i64 descriptors_length);
void resource_manager_register_loader(ResourceLoader *loader);

Resource *resource_manager_load(sid resource_id);
Resource *resource_manager_get(sid resource_id);
void resource_manager_unload(sid resource_id);

#endif // ResourceManager_h
