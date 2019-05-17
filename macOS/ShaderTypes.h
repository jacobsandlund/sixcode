#ifndef ShaderTypes_h
#define ShaderTypes_h

#include "../src/sixcode.h"

// Buffer index values shared between shader and C code to ensure Metal shader buffer inputs match
//   Metal API buffer set calls
typedef enum VertexInputIndex
{
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize = 1,
} VertexInputIndex;

//  This structure defines the layout of each vertex in the array of vertices set as an input to our
//    Metal vertex shader.  Since this header is shared between our .metal shader and C code,
//    we can be sure that the layout of the vertex array in the code matches the layout that
//    our vertex shader expects
typedef struct
{
    // Positions in pixel space (i.e. a value of 100 indicates 100 pixels from the origin/center)
    float2 position;

    // Floating point RGBA colors
    float4 color;
} Vertex;

#endif /* ShaderTypes_h */
