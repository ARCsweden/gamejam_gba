#include "gfx.h"

#include "gamestate.h"
#include "sprite_manager.h"

#include "art.h"

#include "player.h"
#include "art.h"

#include <gbdk/metasprites.h>
#include <gb/cgb.h>
#include "assets/sand.h"

const uint8_t bg_tile = 0;
const uint8_t bg_tile_row[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t bg_attr_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t bg_attr = 1;


void init_gfx(void) {
    SPRITES_8x16;

    init_player_gfx();

    // Initialize tiles (note sprite and bkg tiles 128-255 overlap)
    set_bkg_data(0, 1, sand_tiles);
    set_bkg_palette(S_PAL(1), 1, sand_palettes);

    for(int i = 0; i < 32; i++) {
        set_bkg_tiles(0, i, 32, 1, bg_tile_row);
        set_bkg_attributes(0, i, 32, 1, bg_attr_row);
    }

    init_sprite_manager();

    // Activating layers
    SHOW_BKG;
    SHOW_SPRITES;
    enable_interrupts();
    //SHOW_WIN;
    DISPLAY_ON;
}

void update_gfx(void) {
    // Render player wizard
    move_metasprite_ex(carpet_metasprites[0], carpet_TILE_ORIGIN, 0, P_CARPET_SPRITE, state.player.pos_x.h,  state.player.pos_y.h);
    move_metasprite_ex(wizard_metasprites[0], wizard_TILE_ORIGIN, 0, P_WIZARD_SPRITE, state.player.pos_x.h,  state.player.pos_y.h - 3);
    // Render boss wizard
    move_metasprite_ex(carpet_metasprites[0], carpet_TILE_ORIGIN, 0, E_CARPET_SPRITE, state.boss.pos_x.h,  state.boss.pos_y.h);
    move_metasprite_ex(wizard_metasprites[0], wizard_TILE_ORIGIN, 0, E_WIZARD_SPRITE, state.boss.pos_x.h,  state.boss.pos_y.h + 3);
    
    
    // Scroll background
    scroll_bkg(0,-1);
}
