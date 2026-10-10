#include "gfx.h"

#include "gamestate.h"

/*
#include "letters.h"
#include "guy.h"
#include "map.h"
*/

#include <gbdk/metasprites.h>
#include <gb/cgb.h>
#include "assets/sand.h"

const uint8_t ProfessorOak_tiles[64]={
    0x07,0x00,0x38,0x07,0x16,0x09,0x19,0x06,0x30,0x0f,0x54,0x2f,0x43,0x3d,0x52,0x3d,
    0x30,0x0f,0x69,0x17,0x97,0x68,0x93,0x6c,0x73,0x0d,0x23,0x1c,0x1f,0x00,0x3e,0x00,
    0xe0,0x00,0x10,0xe0,0xe8,0x10,0x18,0xe0,0x0c,0xf0,0x2a,0xf4,0xc2,0xbc,0x4a,0xbc,
    0x0c,0xf0,0x96,0xe8,0xe9,0x16,0xc9,0x36,0xce,0xb0,0xc4,0x38,0xf8,0x00,0x7c,0x00
};

const palette_color_t ProfessorOak_palettes[4] = {
	RGB8(0,0,0), RGB8(0,0,0), RGB8(255,255,255), RGB8(128,128,128)
};

const metasprite_t ProfessorOak_metasprite[] = {
    {.dy=-8, .dx=-8, .dtile=0, .props=S_PAL(1)},
    {.dy=0, .dx=8, .dtile=2, .props=S_PAL(1)},
	METASPR_TERM
};



// Note, sprite indexes
//#define GUY_TILE 0
#define LETTERS_PAL S_PAL(0)
#define LETTERS_SPRITE 4
#define OAK_TILE_ORIGIN 0
#define OAK_PAL S_PAL(1)
#define OAK_SPRITE 0

const uint8_t bg_tile = 0;
const uint8_t bg_tile_row[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t bg_attr_row[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t bg_attr = 1;


void init_gfx(void) {
    SPRITES_8x16;

    // Initialize tiles (note sprite and tile 128-255 overlap)
    // set_bkg_data(first_tile, num_tiles, data)
    set_bkg_data(0, 1, sand_tiles);
    set_bkg_palette(S_PAL(1), 1, sand_palettes);

    for(int i = 0; i < 32; i++) {
        set_bkg_tiles(0, i, 32, 1, bg_tile_row);
        set_bkg_attributes(0, i, 32, 1, bg_attr_row);
    }

    
    // set_bkg_tiles(x, y, w, h, tilemap)
    // set_sprite_data(first_tile, num_tiles, data)
    // set_sprite_tile(id, tile)
    //set_sprite_data(GUY_TILE,1,TileLabel);

    // Oak has 4 tiles, put them in VRAM
    set_sprite_data(OAK_TILE_ORIGIN,4,ProfessorOak_tiles);
    set_sprite_palette(S_PAL(1), 1, ProfessorOak_palettes);

    /*
    set_sprite_data(letters_TILE_ORIGIN,letters_TILE_COUNT,letters_tiles);
    set_sprite_palette(S_PAL(0), letters_PALETTE_COUNT, letters_palettes);

    set_bkg_palette(S_PAL(0), 1, map_palettes);
    set_bkg_data(map_TILE_ORIGIN, map_TILE_COUNT, map_tiles);

    // Set the tiles layer in VRAM
    set_bkg_submap(0, 0, 32, 32, map_map, map_WIDTH/map_TILE_W);
    // Set the attributes layer in VRAM
    set_bkg_submap_attributes(0, 0, 32, 32, map_map_attributes, map_MAP_ATTRIBUTES_WIDTH);
    */

    // Activating layers
    SHOW_BKG;
    SHOW_SPRITES;
    enable_interrupts();
    //SHOW_WIN;
    DISPLAY_ON;
}

void update_gfx(void) {
    //gotogxy(x, y);
    //color(BLACK, WHITE, SOLID);
    //gprint("@");

    // Game main loop processing goes here
    //move_metasprite_ex(letters_metasprites[state.letters_tile], letters_TILE_ORIGIN, 0, LETTERS_SPRITE, 50, 140);

//    move_sprite(GUY_TILE,state.oak_x.h, state.oak_y.h);
    move_metasprite_ex(ProfessorOak_metasprite, OAK_TILE_ORIGIN, 0, OAK_SPRITE, state.oak_x.h, state.oak_y.h);
    // Note to use `fixed` type if wanting to move things less than one pixel each frame

    // Move sprites
    // scroll_sprite(id, x, y); move_sprite(id, x, y);
    // Move BKG layer offset
    // scroll_bkg(x, y); move_bkg(x, y);
    scroll_bkg(0,-1);
    // Text (drawing.h)
    // gotogxy(x, y), color(f, b, mode), gprint/gprintn/gprintln/gprintf

}
