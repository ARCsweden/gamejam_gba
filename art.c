#include "art.h"

#include "sprite_manager.h"

void init_player_gfx(void) {
    set_sprite_data(carpet_TILE_ORIGIN, carpet_TILE_COUNT, carpet_tiles);
    set_sprite_palette(S_PAL(0), 1, carpet_palettes);
    set_sprite_data(wizard_TILE_ORIGIN, wizard_TILE_COUNT, wizard_tiles);
    set_sprite_palette(S_PAL(1), 1, wizard_palettes);
    set_sprite_data(p_spell_TILE_ORIGIN, p_spell_TILE_COUNT, p_spell_tiles);
    set_sprite_palette(S_PAL(2), 1, p_spell_palettes);
    set_sprite_palette(S_PAL(3), 1, e_spell_palettes);
    set_sprite_data(boss_TILE_ORIGIN, boss_TILE_COUNT, boss_tiles);
    set_sprite_palette(S_PAL(4), 1, boss_palettes);
    // Only load tiles for shield, shares S_PAL(2) with player spell
    set_sprite_data(shield_TILE_ORIGIN, shield_TILE_COUNT, shield_tiles);
    set_sprite_data(shield_powerup_TILE_ORIGIN, shield_powerup_TILE_COUNT, shield_powerup_tiles);
}

void init_ui_gfx(void) {
    set_win_data(heart_TILE_ORIGIN, heart_TILE_COUNT, heart_tiles);
    set_win_data(heart_filled_TILE_ORIGIN, heart_filled_TILE_COUNT, heart_filled_tiles);
    const palette_color_t ui_palette[4] = {
	RGB8(255,255,255), RGB8( 58,255, 91), RGB8(255,  0, 25), RGB8(  0,  0,  0)
	};
    set_bkg_palette(S_PAL(0), 1, ui_palette);
}

void init_font(void) {
    set_bkg_data(Font_TILE_ORIGIN, Font_TILE_COUNT, Font_tiles);
}

void init_boss_gfx(void) {
    //tbd
}
