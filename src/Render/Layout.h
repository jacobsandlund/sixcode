#ifndef _RenderLayout_h
#define _RenderLayout_h

#include "Spacetime.h"

typedef enum {
    RenderLayoutTypeHex = 0,
    RenderLayoutTypeRect = 1,
} RenderLayoutType;

#define RenderLayoutTypeNumTypes 2

typedef struct {
    double2 scale;
    RenderLayoutType type;
} RenderLayout;

void RenderLayoutSetType(RenderLayout *rl, RenderLayoutType type);

#endif // _RenderLayout_h
