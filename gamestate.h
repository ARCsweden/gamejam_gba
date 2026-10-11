#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>
#include <gb/gb.h>
#include "boss.h"
#include "player.h"
#include "shield.h"

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

#define PLAYER_MAX_HP 5

#define SHIELD_MOVE_SPEED TO_FIXED(1, 0)
#define SHIELD_COOLDOWN 300 // 5s
#define SHIELD_DURATION 180 // 3s (MAX is 255)

#define HIT_FX_FRAMES 10

struct gamestate_t {
    struct player_t player;
    struct boss_t boss;
    shield_powerup_t shield_powerup;
};

extern struct gamestate_t state;

void init_gamestate(void);

#endif
