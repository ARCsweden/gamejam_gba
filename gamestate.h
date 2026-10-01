#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>
#include <gb/gb.h>

#define TO_FIXED(i, f) (((i) << 8) | (f))

#define SPEED TO_FIXED(1, 0)

struct gamestate_t {
    fixed oak_x;
    fixed oak_y;
    uint8_t letters_tile;
};

#endif
