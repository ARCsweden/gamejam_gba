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

struct gamestate_t {
    struct player_t player;
    struct boss_t boss;
};

extern struct gamestate_t state;

void init_gamestate(void);

#endif
