#include "engine/all-resources.h"
#include "render/layout.h"
#include "model/mesh.h"
#include "resource/gpu.h"

#define AllResourcesCapacity 128

void all_resources_load_descriptors(ResourceManager *rm)
{
    ResourceDescriptor[AllResourcesCapacity] descriptors;
    i64 descriptor_i = 0;

    i64 mesh_size = 32;
    static GpuBufferOptions fill_vertices_buffer_hex_options = {
        .resource = {
            GpuResourceStorageModePrivate,
            fill_mesh_vertices_hex_set,
            mesh_size,
        },
        .length = fill_mesh_vertices_hex_length(mesh_size),
    }
    descriptors[descriptor_i++] = {
        .id = string_id("fill_vertices_buffer_hex"),
        .dependencies = {},
        .type = GpuResourceTypeBuffer,
        .options = (uintptr_t) fill_vertices_buffer_hex_options,
    };

    resource_manager_load_descriptors(rm, AllResources, AllResourcesLength);
}
