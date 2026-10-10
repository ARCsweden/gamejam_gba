#ifndef PLAYER_H
#define PLAYER_H

#include <gb/gb.h>

struct player_t {
    fixed pos_x;
    fixed pos_y;
    uint8_t spell_cooldown;
};

#endif
