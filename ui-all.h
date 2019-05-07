#ifndef __UI_ALL_H__
#define __UI_ALL_H__

#include "sixcode.h"
#include "grid.h"
#include "ui-fill.h"
#include "view.h"

typedef struct {
	UiFill fill;
} UiAll;

bool ui_all_initialize(UiAll *ui);
void ui_all_terminate(UiAll *ui);
void ui_all_draw(UiAll *ui, View *vw, Grid *g);

#endif // __UI_ALL_H__
