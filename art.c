#include "art.h"
#include "font.h"

#include "sprite_manager.h"

const palette_color_t boss_hit_palettes[4] = {
	RGB8(  0,  0,  0), RGB8(255, 0, 230), RGB8(179, 0, 150), RGB8(102, 0, 65)
	};

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
	RGB8(255,255,255), RGB8(0, 0, 0), RGB8(255, 0, 25), RGB8(  0,  0,  0)
	};
    set_bkg_palette(S_PAL(0), 1, ui_palette);
}

void init_font(void) {
    set_bkg_data(Font_TILE_ORIGIN, Font_TILE_COUNT, Font_tiles);
    // Font color 0 is the background; color 2 forms the letter strokes.
    const palette_color_t player_text_palette[4] = {
        Font_palettes[0], wizard_palettes[2], wizard_palettes[1], wizard_palettes[3]
    };
    const palette_color_t boss_text_palette[4] = {
        Font_palettes[0], boss_palettes[2], boss_palettes[1], boss_palettes[3]
    };
    set_bkg_palette(FONT_PALETTE_PLAYER, 1, player_text_palette);
    set_bkg_palette(FONT_PALETTE_BOSS, 1, boss_text_palette);
}

void init_boss_gfx(void) {
    //tbd
}
