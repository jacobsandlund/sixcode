#include "test.h"
#include "engine/all-resources.c"
#include "resource/manager.c"

Test(all_resources_load_descriptors)
{
    ResourceManager *rm = malloc(sizeof *rm);
    resource_manager_init(rm, 64);

    all_resource_descriptors_load_descriptors(rm);

    _s(rm->resource_ids[0]);
    _s(rm->resources[0].id);

    resource_manager_destroy(rm);
    free(rm);
}
