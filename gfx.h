#ifndef GFX_H
#define GFX_H

#include "gamestate.h"

#include <gb/gb.h>
#include <gb/drawing.h>

void init_gfx(void);

void update_gfx(struct gamestate_t state);

#endif
