#include "gfx.h"

#include "gamestate.h"
#include "sprite_manager.h"

#include "art.h"

/*
#include "letters.h"
#include "guy.h"
#include "map.h"
*/
#include "player.h"
#include "art.h"

#include <gbdk/metasprites.h>
#include <gb/cgb.h>

// Note, sprite indexes
//#define GUY_TILE 0
#define LETTERS_PAL S_PAL(0)
#define LETTERS_SPRITE 4
#define OAK_TILE_ORIGIN 0
#define OAK_PAL S_PAL(1)
#define OAK_SPRITE 0

void init_gfx(void) {
    SPRITES_8x16;

    init_player_gfx();

    init_sprite_manager();

    // Activating layers
    SHOW_BKG;
    SHOW_SPRITES;
    enable_interrupts();
    //SHOW_WIN;
    DISPLAY_ON;
}

void update_gfx(void) {
    move_metasprite_ex(carpet_metasprites[0], carpet_TILE_ORIGIN, 0, P_CARPET_SPRITE, state.player.pos_x.h,  state.player.pos_y.h);
    move_metasprite_ex(wizard_metasprites[0], wizard_TILE_ORIGIN, 0, P_WIZARD_SPRITE, state.player.pos_x.h,  state.player.pos_y.h);
}
