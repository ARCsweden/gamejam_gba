#ifndef ART_H
#define ART_H

#include "assets/carpet.h"
#include "assets/wizard.h"
#include "assets/p_spell.h"
#include "assets/boss.h"
#include "assets/shield.h"
#include "assets/shield_powerup.h"

#include "assets/heart.h"
#include "assets/heart_filled.h"
#include "assets/Font.h"

// SPRITE TILES:
// 0: Carpet (4 tiles)
// 4: Wizard (12 tiles)
// 16: Player spell (2 tiles)
// 18: Boss (12 tiles)
// 30: Shield (2 tiles)
// 32: Heart (2 tiles)
// 34: Heart filled (2 tiles)
// 36: Shield powerup (4 tiles)
// 40: ?



// SPRITE PALETTES:
// 0: Carpet
// 1: Wizard(player)
// 2: Spell(player) + shield
// 3: Spell(enemy)
// 4: Boss /boss_hit
// 5:
// 6:
// 7:


// BKG PALETTES:
// 0: UI layer
// 1: Sand
// 2:
// 3:
// 4:
// 5:
// 6:
// 7:

extern const palette_color_t boss_hit_palettes[4];

void init_player_gfx(void);
void init_ui_gfx(void);
void init_font(void);

#endif
