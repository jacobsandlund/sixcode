#ifndef _WorldVector_h
#define _WorldVector_h

#include "Render/Layout.h"
#include "Spacetime.h"

int2 WorldVectorRoundHex(float2 v);
int2 WorldVectorRoundRect(float2 v);
int2 WorldVectorRound(RenderLayout *rl, float2 v);

#endif  // _WorldVector_h
