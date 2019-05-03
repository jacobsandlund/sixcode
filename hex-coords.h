#ifndef __HEX_COORDS_H__
#define __HEX_COORDS_H__

#include "sixcode.h"

ivec2 hex_coords_round(vec2 v);
ivec2 hex_coords_to_storage(ivec2 h);
ivec2 hex_coords_from_storage(ivec2 h);

#endif // __HEX_COORDS_H__
