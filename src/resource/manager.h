#ifndef ResourceManager_h
#define ResourceManager_h

#include "spacetime.h"
#include "resource/resource.h"
#include "resource/pointer-allocator.h"
#include "string/id-table.h"

#define ResourceManagerMaxTopLevelResources 4

typedef struct {
    void *pointer;
    i8 needed_count;
} ResourceManagerResource;

typedef struct {
    i32 loader_capacity;
    i32 resource_capacity;
    i32 descriptor_capacity;
    i32 pointer_allocator_capacity;
} ResourceManagerConfiguration;

typedef struct {
    StringIdTable resource_loaders;
    StringIdTable resource_descriptors;
    StringIdTable resources;
    sid top_level_resource_ids[ResourceManagerMaxTopLevelResources];
    ResourcePointerAllocator pointer_allocator;
} ResourceManager;

extern ResourceManager gResourceManager;

#define ResourcePointerAlloc(size) resource_pointer_allocator_alloc(&gResourceManager.pointer_allocator, size);

void resource_manager_init(ResourceManagerConfiguration *config);
void resource_manager_destroy(void);

void resource_manager_register_loader(sid resource_type, ResourceLoader *loader);
void resource_manager_load_descriptors(const ResourceDescriptor *descriptors, i64 descriptors_length);

void *resource_manager_load(sid resource_id);
void *resource_manager_get(sid resource_id);
void resource_manager_unload(sid resource_id);

#endif // ResourceManager_h
