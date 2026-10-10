#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>
#include <gb/gb.h>
#include "boss.h"
#include "player.h"

#define TO_FIXED(i, f) (((i) << 8) | (f))

#define SPEED_SIDEWAYS TO_FIXED(1, 0)
#define SPEED_UP TO_FIXED(0, 160)
#define SPEED_DOWN TO_FIXED(1, 85)

#define PLAYER_SPELL_SPEED TO_FIXED(4, 0);
#define PLAYER_SPELL_COOLDOWN 10

#define BOSS_SPELL_SPEED TO_FIXED(2, 0);
#define BOSS_SPELL_COOLDOWN 5

// Pixels from bottom
#define PLAYER_START_Y 24

// Boss wave motion
#define BOSS_CENTER_Y 32 // Pixels from top
#define BOSS_ACC_Y 8 // Note, fixed fractional
#define BOSS_PEAK_Y_SPEED 1 // Note, fixed integer

struct gamestate_t {
    struct player_t player;
    struct boss_t boss;
};

extern struct gamestate_t state;

void init_gamestate(void);

#endif
