#import "gpu/metal/primitive.h"

@import MetalKit;

// Order must match GpuPrimitiveType in gpu/primitive.h
const i32 MetalPrimitiveTypeLookup[] = {
    MTLPrimitiveTypeTriangle,
};
