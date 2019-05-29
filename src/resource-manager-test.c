#include "test.h"
#include "resource-manager.c"
#include "resource.c"
#include "string-manager.c"
#include "fnv.c"
#include "string-id.c"
#include "sid-hash-table.c"

Test(resource_manager_start_up_and_shut_down)
{
	resource_manager_start_up(3, 5);

	_d(gResourceManager.resources.size);
	//=> 4
	_d(gResourceManager.resource_descriptors.size);
	//=> 8

	resource_manager_shut_down();
}

Test(resource_manager_load_descriptors)
{
	string_manager_start_up(3);
	resource_manager_start_up(3, 3);

	SidHashTable *descriptors_table = &gResourceManager.resource_descriptors;

	ResourceDescriptor descriptors[] = {
		{
			.id = StringIdIntern("foo"),
			.dependencies = {},
			.type = ResourceTypeList,
			.options = 42,
		},
		{
			.id = StringIdIntern("bar"),
			.dependencies = {},
			.type = ResourceTypeList,
			.options = 123456789,
		},
		{
			.id = StringIdIntern("foobar"),
			.dependencies = {
				StringId("foo"),
				StringId("bar"),
			},
			.type = ResourceTypeList,
			.options = 0,
		},
	};

	resource_manager_load_descriptors(descriptors, 3);

	_d(descriptors_table->count);
	//=> 3

	ResourceDescriptor *rd = sid_hash_table_get(descriptors_table, StringId("foobar"));
	_d(rd->id == StringId("foobar"));
	//=> 1

	resource_manager_shut_down();
	string_manager_shut_down();
}

Test(resource_manager_register_loader)
{
	ResourceLoader list_loader = {
		.type = ResourceTypeList,
		.init = ResourceDefaultInitFn,
		.destroy = ResourceDefaultDestroyFn,
	};

	resource_manager_register_loader(&list_loader);

	_d(gResourceManager.resource_loaders[ResourceTypeList].init == ResourceDefaultInitFn);
	//=> 1
}
