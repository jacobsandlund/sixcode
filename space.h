#ifndef __SPACE_H__
#define __SPACE_H__

#include "sixcode.h"
#include "vector.h"
#include "hex.h"
#include "view.h"

vec2 space_screen_to_world(View *vw, vec2 v);
vec2 space_world_to_hex(vec2 v);
vec2 space_hex_to_world(vec2 v);
Hex space_hex_round(vec2 v);
vec2 space_hex_to_vec(Hex h);
Hex space_hex_to_storage(Hex h);
Hex space_storage_to_hex(Hex h);

#endif // __SPACE_H__
