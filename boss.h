#ifndef BOSS_H
#define BOSS_H

#include <gb/gb.h>


struct boss_t {
    fixed pos_x;
    fixed pos_y;
};

void update_boss_logic();


#endif