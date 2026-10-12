#include "gfx.h"

#include "gamestate.h"
#include "sprite_manager.h"
#include "player.h"
#include "shield.h"
#include "art.h"
#include "rand.h"
#include "font.h"

#include <gbdk/metasprites.h>
#include <gb/cgb.h>
#include "assets/sand.h"
#include "assets/sand_cactus_1.h"
#include "assets/sand_cactus_2.h"

#define RAND_RANGE_8BIT(randval, range) (uint8_t)(((randval & 0xFFu) * range) >> 8)

const uint8_t bg_tile = 0;
uint8_t bg_tile_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t bg_attr_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t bg_attr = 1;

void draw_health(void);
void draw_boss_hp_bar(void);

void init_gfx(void) {
    SPRITES_8x16;


    init_player_gfx();
    init_ui_gfx();
    init_font();
    move_win(7, 136);

    uint16_t seed;
    seed = DIV_REG;
    seed |= (UWORD)DIV_REG << 8;

    initarand(seed);


    // Initialize tiles (note sprite and bkg tiles 128-255 overlap)
   
    set_bkg_data(sand_TILE_ORIGIN, 1, sand_tiles);
    set_bkg_palette(S_PAL(1), 1, sand_palettes);
 
    set_bkg_data(sand_cactus_1_TILE_ORIGIN, 1, sand_cactus_1_tiles);
    set_bkg_palette(S_PAL(2), 1, sand_cactus_1_palettes);

    set_bkg_data(sand_cactus_2_TILE_ORIGIN, 1, sand_cactus_2_tiles);
    set_bkg_palette(S_PAL(3), 1, sand_cactus_2_palettes);
    
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 20; j++){
            bg_tile_row[j] = RAND_RANGE_8BIT(rand(), 100) < 2 ? 2 : RAND_RANGE_8BIT(rand(), 100) < 2 ? 3 : 1;
            bg_attr_row[j] = bg_tile_row[j];
        }
        set_bkg_tiles(0, i, 20, 1, bg_tile_row);
        set_bkg_attributes(0, i, 20, 1, bg_attr_row);
    }

    init_sprite_manager();

    // Activating layers
    SHOW_BKG;
    SHOW_SPRITES;
    enable_interrupts();
    SHOW_WIN;
    DISPLAY_ON;

}

#define END_STATE_WON 0x1
#define END_STATE_LOST 0x2
uint8_t end_state = 0;

const char lost_text[] = "You lost!";
const char won_text[] = "You won!";

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
        if(~end_state & END_STATE_LOST) {
            end_state |= END_STATE_LOST;
            render_text(8, 0, lost_text);
        }
    }

    // Render player shield
    if(state.player.shield) {
        move_metasprite_ex(shield_metasprites[0], shield_TILE_ORIGIN, 0, P_SHIELD_SPRITE, state.player.pos_x.h, state.player.pos_y.h - 16);
    } else {
        hide_sprite(P_SHIELD_SPRITE);
        hide_sprite(P_SHIELD_SPRITE + 1);
    }

    // Render Health UI (but only when updated)
    if(state.player.hp != state.player.hp_prev) {
        draw_health();
        state.player.hp_prev = state.player.hp;
    }
    if(state.boss.hp > 0 && (state.boss.hp != state.boss.hp_prev) && state.player.hp > 0) {
        draw_boss_hp_bar();
        state.boss.hp_prev = state.boss.hp;
    }

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
        
        if(~end_state & END_STATE_WON) {
            end_state |= END_STATE_WON;
            render_text(8, 0, won_text);
        }
    }

    // Scroll background
    scroll_bkg(0,-1);

    draw_projectiles();
    draw_shield_powerup();
}

void draw_health(void) {
    move_win(7, 136);
    for(uint8_t i = 0; i < PLAYER_MAX_HP; ++i) {
        const uint8_t* map = (state.player.hp > i) ? heart_filled_map : heart_map;
        set_win_tiles(i, 0, 1, 1, map);
    }
}

void draw_boss_hp_bar(void) {
    uint8_t segments = state.boss.hp >> 2;
    uint8_t current_tile = 11;
    uint8_t current_segment;

    for(uint8_t i = 0; i < 8; ++i) {
        current_segment = (1+i) * 8;
        if(segments < current_segment) {
            current_tile = segments % 8;
        }
        if(segments + 8 < current_segment) {
            set_win_tiles(18-i, 0, 1, 1, &Font_map[0]);
        } else {
            set_win_tiles(18-i, 0, 1, 1, &boss_health_bar_map[current_tile]);
        }
    }

    // set_win_tiles(11, 0, 1, 1, &boss_health_bar_map[11]);
    // set_win_tiles(12, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(13, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(14, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(15, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(16, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(17, 0, 1, 1, &boss_health_bar_map[0]);
    // set_win_tiles(18, 0, 1, 1, &boss_health_bar_map[0]);

    set_win_tiles(19, 0, 1, 1, &boss_health_bar_map[10]);
}
