#include "resource/manager.h"
#include <assert.h>
#include <stdlib.h>
#include "string/manager.h"
#include "log/manager.h"

ResourceManager gResourceManager;

void resource_manager_init(ResourceManagerConfig *config)
{
    string_id_table_init(&gResourceManager.resource_loaders,
            config->loader_capacity, sizeof(ResourceLoader));
    string_id_table_init(&gResourceManager.resource_descriptors,
            config->descriptor_capacity, sizeof(ResourceDescriptor));
    string_id_table_init(&gResourceManager.resources,
            config->resource_capacity,
            sizeof(ResourceManagerResource));

    for (i32f i = 0; i < ResourceManagerMaxTopLevelResources; i++) {
        gResourceManager.top_level_resource_ids[i] = 0;
    }

    resource_pointer_allocator_init(&gResourceManager.pointer_allocator, config->pointer_allocator_capacity);
}

void resource_manager_destroy(void)
{
    for (i32f i = 0; i < ResourceManagerMaxTopLevelResources; i++) {
        sid resource_id = gResourceManager.top_level_resource_ids[i];
        if (resource_id) {
            resource_manager_unload(resource_id);
        }
    }

    string_id_table_destroy(&gResourceManager.resource_loaders);
    string_id_table_destroy(&gResourceManager.resource_descriptors);
    string_id_table_destroy(&gResourceManager.resources);
    resource_pointer_allocator_destroy(&gResourceManager.pointer_allocator);
}

void resource_manager_register_loader(sid resource_type, ResourceLoader *loader)
{
    string_id_table_insert(&gResourceManager.resource_loaders, resource_type, loader);
}

void resource_manager_load_descriptors(const ResourceDescriptor *descriptors, i64 descriptors_length)
{
    StringIdTable *descriptors_table = &gResourceManager.resource_descriptors;
    assert(descriptors_table->count + descriptors_length <= descriptors_table->size);

    for (i64 i = 0; i < descriptors_length; i++) {
        string_id_table_insert(descriptors_table,
                descriptors[i].id, &descriptors[i]);
    }
}

static ResourceDescriptor *resource_manager_get_descriptor(sid resource_id)
{
    ResourceDescriptor *descriptor = string_id_table_get(&gResourceManager.resource_descriptors, resource_id);

    if (!descriptor) {
        u8 *string = StringIdString(resource_id);
        LogDefault(&gLogManager.logs.engine,
                "No descriptor found for resource: '%s' (resource_id: %"
                PRIu64 ")", string, resource_id);
        abort();
    }

    return descriptor;
}

static ResourceLoader *resource_manager_get_loader(sid resource_type)
{
    ResourceLoader *loader = string_id_table_get(&gResourceManager.resource_loaders, resource_type);

    if (!loader) {
        u8 *string = StringIdString(resource_type);
        LogDefault(&gLogManager.logs.engine,
                "No loader found for resource type: '%s' (resource_type_id: %"
                PRIu64 ")", string, resource_type);
        abort();
    }

    return loader;
}

static void *resource_manager_load_inner(sid resource_id)
{
    StringIdTable *resources = &gResourceManager.resources;
    ResourceManagerResource *manager_resource = string_id_table_get(resources, resource_id);
    if (manager_resource) {
        manager_resource->needed_count++;
        return manager_resource->pointer;
    }

    ResourceDescriptor *descriptor = resource_manager_get_descriptor(resource_id);

    Resource resource = {.id = resource_id};

    for (i32f i = 0; i < ResourceMaxNumDependencies; i++) {
        sid dependency_id = descriptor->dependencies[i];
        if (dependency_id) {
            resource.dependencies[i] = resource_manager_load_inner(dependency_id);
        }
    }

    ResourceLoader *loader = resource_manager_get_loader(descriptor->type);
    loader->init(&resource, descriptor->options);
    ResourceManagerResource mr = {
        .pointer = resource.pointer,
        .needed_count = 1,
    };
    string_id_table_insert(resources, resource_id, &mr);

    return resource.pointer;
}

void *resource_manager_load(sid resource_id)
{
    for (i32f i = 0; i < ResourceManagerMaxTopLevelResources; i++) {
        if (resource_id == gResourceManager.top_level_resource_ids[i]) {
            return resource_manager_get(resource_id);
        }
    }

    for (i32f i = 0; i < ResourceManagerMaxTopLevelResources; i++) {
        if (!gResourceManager.top_level_resource_ids[i]) {
            gResourceManager.top_level_resource_ids[i] = resource_id;
            return resource_manager_load_inner(resource_id);
        }
    }

    u8 *string = StringIdString(resource_id);
    LogDefault(&gLogManager.logs.engine,
            "Attempt to load top level resource: '%s' (resource_id: %"
            PRIu64 "), but maximum of %d top level resources already loaded",
            string, resource_id, ResourceManagerMaxTopLevelResources);
    abort();
}

void *resource_manager_get(sid resource_id)
{
    ResourceManagerResource *manager_resource = string_id_table_get(&gResourceManager.resources, resource_id);

    if (!manager_resource) {
        u8 *string = StringIdString(resource_id);
        LogDefault(&gLogManager.logs.engine,
                "Trying to get unloaded resource : '%s' (resource_id: %"
                PRIu64 ")", string, resource_id);
        abort();
    }

    return manager_resource->pointer;
}

static void resource_manager_unload_inner(sid resource_id)
{
    StringIdTable *resources = &gResourceManager.resources;
    ResourceManagerResource *manager_resource = string_id_table_get(resources, resource_id);

    if (!manager_resource || (--manager_resource->needed_count) > 0) {
        return;
    }

    ResourceDescriptor *descriptor = resource_manager_get_descriptor(resource_id);
    ResourceLoader *loader = resource_manager_get_loader(descriptor->type);
    Resource resource = {
        .id = descriptor->id,
        .pointer = manager_resource->pointer,
    };

    for (i32f i = 0; i < ResourceMaxNumDependencies; i++) {
        sid dependency_id = descriptor->dependencies[i];
        if (dependency_id) {
            resource.dependencies[i] = resource_manager_get(dependency_id);
        }
    }

    loader->destroy(&resource);
    string_id_table_delete(resources, resource_id);

    for (i32f i = 0; i < ResourceMaxNumDependencies; i++) {
        sid dependency_id = descriptor->dependencies[i];
        if (dependency_id) {
            resource_manager_unload_inner(dependency_id);
        }
    }
}

void resource_manager_unload(sid resource_id)
{
    for (i32f i = 0; i < ResourceManagerMaxTopLevelResources; i++) {
        if (resource_id == gResourceManager.top_level_resource_ids[i]) {
            gResourceManager.top_level_resource_ids[i] = 0;
            resource_manager_unload_inner(resource_id);
        }
    }
}
