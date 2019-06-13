#include "resource/manager.c"
#include "log/manager-mock.c"
#include "math/fnv.c"
#include "resource/pointer-allocator.c"
#include "resource/resource.c"
#include "string/id-table.c"
#include "string/id.c"
#include "string/manager.c"
#include "test.h"

static ResourceManagerConfig test_resource_manager_config = {
    .loader_capacity = 2,
    .resource_capacity = 4,
    .descriptor_capacity = 8,
    .pointer_allocator_capacity = 128,
};

static StringManagerConfig test_string_manager_config = {
    .string_table_size = 8,
};

Test(resource_manager_init)
{
    string_manager_init(&test_string_manager_config);
    resource_manager_init(&test_resource_manager_config);

    _d(gResourceManager.resource_loaders.size);
    //=> 2
    _d(gResourceManager.resources.size);
    //=> 4
    _d(gResourceManager.resource_descriptors.size);
    //=> 8
    _d(gResourceManager.top_level_resource_ids[0]);
    //=> 0
    _d(gResourceManager.top_level_resource_ids[3]);
    //=> 0
    _d(gResourceManager.pointer_allocator.capacity);
    //=> 128
    _d(gResourceManager.pointer_allocator.allocated);
    //=> 0

    resource_manager_destroy();
    string_manager_destroy();
}

Test(resource_manager_register_loader)
{
    string_manager_init(&test_string_manager_config);
    resource_manager_init(&test_resource_manager_config);

    ResourceLoader list_loader = {
        .init = ResourceDefaultInitFn,
        .destroy = ResourceDefaultDestroyFn,
    };

    resource_manager_register_loader(StringIdIntern("test_loader"), &list_loader);

    ResourceLoader *l = string_id_table_get(&gResourceManager.resource_loaders, StringId("test_loader"));
    _d(l->init == ResourceDefaultInitFn);
    //=> 1

    resource_manager_destroy();
    string_manager_destroy();
}

Test(resource_manager_load_descriptors)
{
    string_manager_init(&test_string_manager_config);
    resource_manager_init(&test_resource_manager_config);

    StringIdTable *descriptors_table = &gResourceManager.resource_descriptors;

    ResourceDescriptor descriptors[] = {
        {
            .id = StringIdIntern("foobar"),
            .dependencies = {},
            .type = StringIdIntern("list"),
        },
    };

    resource_manager_load_descriptors(descriptors, 1);

    _d(descriptors_table->count);
    //=> 1

    ResourceDescriptor *rd = string_id_table_get(descriptors_table, StringId("foobar"));
    _d(rd->id == StringId("foobar"));
    //=> 1

    resource_manager_destroy();
    string_manager_destroy();
}

static i32 test_unloaded_count = 0;

typedef struct {
    i64 number;
} TestResource;

static void test_resource_init(Resource *resource, uintptr_t options)
{
    TestResource *test_resource = ResourcePointerAlloc(sizeof *test_resource);
    test_resource->number = (i64) options;
    resource->pointer = test_resource;
    LogDefault(gLogManager.logs.engine,
            "Loaded %s", StringIdString(resource->id));
}

static void test_list_resource_init(Resource *resource, uintptr_t options)
{
    (void) options;
    LogDefault(gLogManager.logs.engine,
            "Loaded %s", StringIdString(resource->id));
}

static void test_resource_destroy(Resource *resource)
{
    LogDefault(gLogManager.logs.engine,
            "Unloaded %s", StringIdString(resource->id));
    resource->pointer = NULL;
    test_unloaded_count++;
}

Test(resource_manager_load_get_unload)
{
    log_manager_init(&gLogManagerMockConfig);

    string_manager_init(&test_string_manager_config);
    resource_manager_init(&test_resource_manager_config);

    ResourceLoader test_loader = {
        .init = test_resource_init,
        .destroy = test_resource_destroy,
    };
    ResourceLoader list_loader = {
        .init = test_list_resource_init,
        .destroy = test_resource_destroy,
    };

    resource_manager_register_loader(StringIdIntern("test"), &test_loader);
    resource_manager_register_loader(StringIdIntern("list"), &list_loader);

    ResourceDescriptor descriptors[] = {
        {
            .id = StringIdIntern("foo"),
            .dependencies = {},
            .type = StringId("test"),
            .options = 42,
        },
        {
            .id = StringIdIntern("bar"),
            .dependencies = {},
            .type = StringId("test"),
            .options = 123456789,
        },
        {
            .id = StringIdIntern("foobar"),
            .dependencies = {
                StringId("foo"),
                StringId("bar"),
            },
            .type = StringId("list"),
        },
    };

    resource_manager_load_descriptors(descriptors, 3);

    test_unloaded_count = 0;

    TestResource *resource = resource_manager_load(StringId("foo"));

    _d(resource->number);
    //=> 42
    ResourceManagerResource *manager_resource = string_id_table_get(&gResourceManager.resources, StringId("foo"));
    _d(manager_resource->needed_count);
    //=> 1

    resource_manager_load(StringId("foobar"));

    _d(manager_resource->needed_count);
    //=> 2

    resource = resource_manager_get(StringId("bar"));

    _d(resource->number);
    //=> 123456789

    resource_manager_unload(StringId("foo"));

    _d(manager_resource->needed_count);
    //=> 1
    _d(test_unloaded_count);
    //=> 0
    _d(string_id_table_get(&gResourceManager.resources, StringId("foo")) != NULL);
    //=> 1

    resource_manager_unload(StringId("foobar"));

    _d(manager_resource->needed_count);
    //=> 0
    _d(test_unloaded_count);
    //=> 3
    _d(string_id_table_get(&gResourceManager.resources, StringId("foo")) != NULL);
    //=> 0

    // All resources are unloaded by destroy

    resource_manager_load(StringId("bar"));
    resource_manager_destroy();
    string_manager_destroy();

    _d(test_unloaded_count);
    //=> 4

    _Log(gLogManager.logs.engine);
    //=> [debug]  Resource allocating 8 bytes (8 bytes total)
    //=> [default]  Loaded foo
    //=> [debug]  Resource allocating 8 bytes (16 bytes total)
    //=> [default]  Loaded bar
    //=> [default]  Loaded foobar
    //=> [default]  Unloaded foobar
    //=> [default]  Unloaded foo
    //=> [default]  Unloaded bar
    //=> [debug]  Resource allocating 8 bytes (24 bytes total)
    //=> [default]  Loaded bar
    //=> [default]  Unloaded bar
    //=>
}
