#ifndef PLAYER_H
#define PLAYER_H

#include <gb/gb.h>

struct player_t {
    fixed pos_x;
    fixed pos_y;
    fixed dead_pos_y;
    uint8_t spell_cooldown;
    uint8_t anim_frame;
    uint8_t shield;
    uint8_t hp;
};

#endif
