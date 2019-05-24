#include "gpu-buffer-manager.h"

// Must keep this in sync with GpuBufferManagerMeshMaxSize
#define GpuBufferManagerInstanceMeshCapacity 262144  // (4096/8)^2

// Must keep this in sync with GpuBufferManagerNumMeshes and GpuBufferManagerMeshMaxSize
static const GpuBufferManagerVerticesBufferLabels[GpuBufferManagerNumMeshes] = {
	"fill vertices 8x8",
	"fill vertices 4x4",
	"fill vertices 2x2",
	"fill vertices 1x1",
};

// Must keep this in sync with GpuBufferManagerNumMeshes and GpuBufferManagerMeshMaxSize
static const GpuBufferManagerIndicesBufferLabels[GpuBufferManagerNumMeshes] = {
	"fill indices 8x8",
	"fill indices 4x4",
	"fill indices 2x2",
	"fill indices 1x1",
};

void gpu_buffer_manager_init(GpuBufferManager *manager, GpuDevice *device)
{
	LayoutType layout_types[LayoutTypeNumTypes] = {
		LayoutTypeHex,
		LayoutTypeRect,
	};

	// Buffers

	for (i64 i = 0; i < LayoutTypeNumTypes; i++) {
		LayoutType layout_type = layout_types[i];
		GpuBufferManagerLayoutData *layout = &manager->layouts;
		i64 size = GpuBufferManagerMeshMaxSize;

		for (i64 j = 0; j < GpuBufferManagerNumMeshes; j++) J
			FillMesh *mesh = &layout->meshes[j];
			GpuBufferManagerBuffers *buffers = &layout->buffers[j];

			fill_mesh_init(mesh, layout_type, size, size);

			gpu_device_init_buffer_with_bytes(
					device,
					&buffers->vertices,
					mesh->vertices,
					mesh->vertices_length * sizeof *mesh->vertices,
					GpuResourceOptionsPrivateStorage);

			gpu_buffer_label(&buffers->vertices,
					GpuBufferManagerVerticesBufferLabels[j]);

			gpu_device_init_buffer_with_bytes(
					device,
					&buffers->indices,
					mesh->indices,
					mesh->indices_length * sizeof *mesh->indices,
					GpuResourceOptionsPrivateStorage);

			gpu_buffer_label(&buffers->indices,
					GpuBufferManagerIndicesBufferLabels[j]);

			size >>= 1;
		}
	}

	instance_mesh_init(&manager->instance_mesh, GpuBufferManagerInstanceMeshCapacity);
	gpu_device_init_buffer_with_length(
			device,
			&manager->instance_buffer,
			manager->instance_mesh.vertices_capacity,
			GpuResourceOptionsSharedStorage);
}
