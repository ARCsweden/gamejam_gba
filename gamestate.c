#include "gamestate.h"

#include "sprite_manager.h"

struct gamestate_t state;

// Pixels from bottom
#define PLAYER_START_Y 16

void init_gamestate(void) {
    // Spawn player at the bottom center of the screen
    const uint8_t player_start_x = SPRITE_LAYER_ORIG_X + (SPRITE_LAYER_SCREEN_LIM_X - SPRITE_LAYER_ORIG_X) / 2;
    const uint8_t player_start_y = (SPRITE_LAYER_SCREEN_LIM_Y - PLAYER_START_Y);
    
    const uint8_t boss_start_x = 80 + SPRITE_LAYER_ORIG_X;
    const uint8_t boss_start_y = SPRITE_LAYER_ORIG_Y + 8;


    state.player.pos_x.w = TO_FIXED(player_start_x, 0);
    state.player.pos_y.w = TO_FIXED(player_start_y, 0);

    state.boss.pos_x.w = TO_FIXED(boss_start_x, 0);
    state.boss.pos_y.w = TO_FIXED(boss_start_y, 0);
  
    state.player.spell_cooldown = 0;
}
