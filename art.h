#ifndef ART_H
#define ART_H

#include "assets/carpet.h"
#include "assets/wizard.h"
#include "assets/p_spell.h"
#include "assets/boss.h"
#include "assets/shield.h"

#include "assets/heart.h"
#include "assets/heart_filled.h"

// SPRITE TILES:
// 0: Carpet (4 tiles)
// 4: Wizard (12 tiles)
// 16: Player spell (2 tiles)
// 18: Boss (12 tiles)
// 30: Shield (2 tiles)
// 32: ?


// SPRITE PALETTES:
// 0: Carpet
// 1: Wizard(player)
// 2: Spell(player) + shield
// 3: Spell(enemy)
// 4: Boss
// 5:
// 6:
// 7:


// BKG PALETTES:
// 0:
// 1:
// 2:
// 3:
// 4:
// 5:
// 6:
// 7:

void init_player_gfx(void);
void init_ui_gfx(void);

#endif
