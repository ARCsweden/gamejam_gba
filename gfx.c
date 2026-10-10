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
const uint8_t bg_tile_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t bg_attr_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t bg_attr = 1;

void draw_health(void);

void init_gfx(void) {
    SPRITES_8x16;

    init_player_gfx();
    init_ui_gfx();
    move_win(7, 136);


    // Initialize tiles (note sprite and bkg tiles 128-255 overlap)
    set_bkg_data(sand_TILE_ORIGIN, 1, sand_tiles);
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
    SHOW_WIN;
    DISPLAY_ON;
}

void update_gfx(void) {
    // Render player wizard
    move_metasprite_ex(carpet_metasprites[0], carpet_TILE_ORIGIN, 0, P_CARPET_SPRITE, state.player.pos_x.h,  state.player.pos_y.h);
    if(state.player.hp > 0) {
        move_metasprite_ex(wizard_metasprites[state.player.anim_frame], wizard_TILE_ORIGIN, 0, P_WIZARD_SPRITE, state.player.pos_x.h, state.player.pos_y.h - 5);
    } else { // Dead
        if(state.player.dead_pos_y.h > SPRITE_LAYER_OOB_Y) {
            hide_sprite(P_WIZARD_SPRITE);
            hide_sprite(P_WIZARD_SPRITE+1);
        } else {
            move_metasprite_ex(wizard_metasprites[state.player.anim_frame], wizard_TILE_ORIGIN, 0, P_WIZARD_SPRITE, state.player.pos_x.h, state.player.dead_pos_y.h - 5);
        }
    }

    // Render player shield
    if(state.player.shield) {
        move_metasprite_ex(shield_metasprites[0], shield_TILE_ORIGIN, 0, P_SHIELD_SPRITE, state.player.pos_x.h, state.player.pos_y.h - 16);
    } else {
        hide_sprite(P_SHIELD_SPRITE);
        hide_sprite(P_SHIELD_SPRITE + 1);
    }

    // Render UI
    draw_health();

    // Render boss wizard
    move_metasprite_ex(carpet_metasprites[0], carpet_TILE_ORIGIN, 0, E_CARPET_SPRITE, state.boss.pos_x.h,  state.boss.pos_y.h);
    if (state.boss.hp > 0) {
        move_metasprite_ex(boss_metasprites[state.player.anim_frame], boss_TILE_ORIGIN, 0, E_WIZARD_SPRITE, state.boss.pos_x.h, state.boss.pos_y.h - 5);
    } else { // Dead
        if(state.player.dead_pos_y.h > SPRITE_LAYER_OOB_Y) {
            hide_sprite(E_WIZARD_SPRITE);
            hide_sprite(E_WIZARD_SPRITE+1);
        } else {
            move_metasprite_ex(boss_metasprites[0], boss_TILE_ORIGIN, 0, E_WIZARD_SPRITE, state.boss.pos_x.h, state.boss.dead_pos_y.h - 5);
        }
    }

    // Scroll background
    scroll_bkg(0,-1);

    draw_projectiles();

    // spawn and remove hitsplat
    gfx_hitsplat_enemy();
    gfx_hitsplat_player();
}

void draw_health(void) {
    move_win(7, 136);
    for(uint8_t i = 0; i < PLAYER_MAX_HP; ++i) {
        const uint8_t* map = (state.player.hp > i) ? heart_filled_map : heart_map;
        set_win_tiles(i, 0, 1, 1, map);
    }
}

void gfx_hitsplat_player(void) {
    //if hit spawn hitsplat and set hitsplat timer
    if (state.player.hit == 1) {
        state.player.hit = 20;
        move_metasprite_ex(hitsplat_metasprites[1], hitsplat_TILE_ORIGIN, 0, P_HITSPLAT_SPRITE, state.player.pos_x.h, state.player.pos_y.h - 4);
    }    
    else if (state.player.hit == 2) {
        hide_sprite(P_HITSPLAT_SPRITE);
        hide_sprite(P_HITSPLAT_SPRITE+1);
    }
    //decrement "timer"
    else {
        state.player.hit--;
    } 
}

void gfx_hitsplat_enemy(void) {
    //if hit spawn hitsplat and set hitsplat timer
    if (state.boss.hit == 1) {
        state.boss.hit = 20;
        move_metasprite_ex(hitsplat_metasprites[0], hitsplat_TILE_ORIGIN, 0, E_HITSPLAT_SPRITE, state.boss.pos_x.h, state.boss.pos_y.h - 4);
    }    
    else if (state.boss.hit == 2) {
        hide_sprite(E_HITSPLAT_SPRITE);
        hide_sprite(E_HITSPLAT_SPRITE+1);
    }
    //decrement "timer"
    else {
        state.boss.hit--;
    } 
}