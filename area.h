#ifndef __AREA_H__
#define __AREA_H__

#include "area-quad.h"
#include "sixcode.h"
#include "quad.h"
#include "vector.h"

#define AREA_ZONE_SIZE 768	// 512 * 1.5 (512 hexes tall)
#define AREA_VERTICES_PER_AREA 8
#define AREA_INDICES_PER_AREA 24
#define AREA_ZONE_MAX_AREAS 8192  // 65536 / 8
#define AREA_ZONE_MIN_CAPACITY 4

typedef struct {
	AreaQuad inner;
	AreaQuad outer;
} Area;

typedef struct {
	Area *areas;
	i32 areas_length;
	i32 areas_capacity;
} AreaZone;

typedef struct {
	AreaQuad quad;
	SizeQuad zone_quad;
	AreaZone *zones;
} AreaGrid;

typedef struct {
	u16 *indices;
	i32 indices_length;
} AreaIndicesMesh;

void area_zone_initialize(AreaZone *az, i32 areas_capacity);
void area_zone_terminate(AreaZone *az);

void area_grid_initialize(AreaGrid *ag, AreaQuad *quad);
void area_grid_terminate(AreaGrid *ag);
void area_grid_add(AreaGrid *ag, AreaQuad *quad);
void area_grid_set(AreaGrid *ag, AreaQuad *old_quad, AreaQuad *new_quad);
void area_grid_remove(AreaGrid *ag, AreaQuad *quad);
i8 area_grid_area_at_point(AreaGrid *ag, AreaQuad *out_quad, vec2 v);
void area_grid_expand_quad(AreaGrid *ag, AreaQuad *quad);

void area_indices_mesh_initialize(AreaIndicesMesh *m, i32 num_areas);
void area_indices_mesh_terminate(AreaIndicesMesh *m);

#endif // __AREA_H__
