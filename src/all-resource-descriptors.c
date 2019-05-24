#include "all-resource-descriptors.h"
#include "layout.h"
#include "mesh.h"
#include "gpu-resource.h"

static const FillMeshOptions FillMeshOptionsHex8 = {LayoutTypeHex, 8};
static const FillMeshOptions FillMeshOptionsHex4 = {LayoutTypeHex, 4};
static const FillMeshOptions FillMeshOptionsHex2 = {LayoutTypeHex, 2};
static const FillMeshOptions FillMeshOptionsHex1 = {LayoutTypeHex, 1};

typedef union {
	GpuResourceOptions gpu_resource_options;
	FillMeshOptions fill_mesh_options;
} AllResourceDescriptorsOptions;

typedef struct {
	char *id;
	char *dependencies;	// Comma separated
	ResourceType type;
	AllResourceDescriptorsOptions options;
} AllResourceDescriptorsDescriptor;

static const AllResourceDescriptorsDescriptor AllResourceDescriptors[] = {
	{
		.id = "fill_vertices_buffer_hex",
		.dependencies = "",
		.type = GpuResourceTypeBuffer,
		.options = {.fill_mesh_options = {
			.layout_type = LayoutTypeHex,
			.size = 8,
		}},
	},
	{
		.id = "fill_mesh_indices_hex",
		.dependencies = "",
		.type = ResourceTypeFillMesh,
		.options = {.fill_mesh_options = {
			.layout_type = LayoutTypeHex,
			.size = 8,
		}},
	},
	{
		.id = "fill_mesh_vertices_rect",
		.dependencies = "",
		.type = ResourceTypeFillMesh,
		.options = {.fill_mesh_options = {
			.layout_type = LayoutTypeRect,
			.size = 8,
		}},
	},
	{
		.id = "fill_vertices_buffer_hex",
		.dependencies = "fill_mesh_hex",
		.type = GpuResourceTypeBuffer,
		.options = {
			.gpu_resource_options = GpuResourceOptionsPrivateStorage,
		},
	},
	{
		.id = "fill_indices_buffer_hex",
		.dependencies = "fill_mesh_hex",
		.type = GpuResourceTypeBuffer,
		.options = {
			.gpu_resource_options = GpuResourceOptionsPrivateStorage,
		},
	},
	{
		.id = "fill_vertices_buffer_rect",
		.dependencies = "fill_mesh_rect",
		.type = GpuResourceTypeBuffer,
		.options = {
			.gpu_resource_options = GpuResourceOptionsPrivateStorage,
		},
	},
	{
		.id = "fill_indices_buffer_rect",
		.dependencies = "fill_mesh_rect",
		.type = GpuResourceTypeBuffer,
		.options = {
			.gpu_resource_options = GpuResourceOptionsPrivateStorage,
		},
	},
};

static const char *AllResourceDescriptorsFillMeshRectIds = {
	"fill_mesh_rect:8",
	"fill_mesh_rect:4",
	"fill_mesh_rect:2",
	"fill_mesh_rect:1",
};

const char *AllResourceDescriptorsFillMeshIds[LayoutTypeNumTypes] = {
	AllResourceDescriptorsFillMeshHexIds,
	AllResourceDescriptorsFillMeshRectIds,
};

static const char *AllResourceDescriptorsFillVerticesBufferHexIds = {
	"fill_vertices_buffer_hex:8",
	"fill_vertices_buffer_hex:4",
	"fill_vertices_buffer_hex:2",
	"fill_vertices_buffer_hex:1",
};

static const char *AllResourceDescriptorsFillVerticesBufferRectIds = {
	"fill_vertices_buffer_rect:8",
	"fill_vertices_buffer_rect:4",
	"fill_vertices_buffer_rect:2",
	"fill_vertices_buffer_rect:1",
};

const char *AllResourceDescriptorsFillVerticesBufferIds[LayoutTypeNumTypes] = {
	AllResourceDescriptorsFillVerticesBufferHexIds,
	AllResourceDescriptorsFillVerticesBufferRectIds,
};

static const char *AllResourceDescriptorsFillIndicesBufferHexIds = {
	"fill_vertices_buffer_hex:8",
	"fill_vertices_buffer_hex:4",
	"fill_vertices_buffer_hex:2",
	"fill_vertices_buffer_hex:1",
};

static const char *AllResourceDescriptorsFillIndicesBufferRectIds = {
	"fill_vertices_buffer_rect:8",
	"fill_vertices_buffer_rect:4",
	"fill_vertices_buffer_rect:2",
	"fill_vertices_buffer_rect:1",
};

const char *AllResourceDescriptorsFillIndicesBufferIds[LayoutTypeNumTypes] = {
	AllResourceDescriptorsFillIndicesBufferHexIds,
	AllResourceDescriptorsFillIndicesBufferRectIds,
};

void all_resource_descriptors_load_descriptors(ResourceManager *rm)
{
	ResourceDescriptor[AllResourceDescriptorsCapacity] descriptors;
	i64 descriptor_i = 0;

	i64 size = AllResourceDescriptorsFillMeshMaxSize;

	for (i64 i = 0; i < AllResourceDescriptorsNumFillMeshes; i++) {
		descriptors[descriptor_i++] = {
			.id = AllResourceDescriptorsFillMeshHexIds[i],
			.dependencies = "",
			.type = ResourceTypeFillMesh,
			.options = {LayoutTypeHex, size},
		}
		descriptors[descriptor_i++] = {
			.id = AllResourceDescriptorsFillMeshRectIds[i],
			.dependencies = "",
			.type = ResourceTypeFillMesh,
			.options = {LayoutTypeRect, size},
		}
		descriptors[descriptor_i++] = {
			.id = AllResourceDescriptorsFillVerticesBufferHexIds[i],
			.dependencies = AllResourceDescriptorsFillMeshHexIds[i],
			.type = ResourceTypeFillMesh,
			.options = {LayoutTypeHex, size},
		}
		descriptors[descriptor_i++] = {
			.id = AllResourceDescriptorsFillVerticesBufferRectIds[i],
			.dependencies = AllResourceDescriptorsFillMeshRectIds[i],
			.type = ResourceTypeFillMesh,
			.options = {LayoutTypeRect, size},
		}

		size >>= 1;
	}
		
	resource_manager_load_descriptors(rm, AllResourceDescriptors, AllResourceDescriptorsLength);
}
