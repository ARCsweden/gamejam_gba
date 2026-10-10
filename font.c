#include "font.h"
#include <gb/cgb.h>

// for each character, if between 101 and 132 (only support writing uppercase), 
// convert to tile pos (A is 33). Numbers are ASCII 60 to 71, 0 is tile pos 16

//uint8_t ascii_to_tile_pos(char);

uint8_t ascii_to_tile_pos(char glyph) {
    uint8_t uppercase_offset = 1;
    uint8_t number_offset = 27;
    char letterA = 'A';
    uint8_t glyphInt = (uint8_t) glyph;
    // It's an uppercase letter
    if(glyphInt >= (uint8_t) 'A' && glyphInt <= (uint8_t) 'Z') {
        return (glyphInt - (uint8_t) 'A' + uppercase_offset);
    } 
    // It's a lowercase letter
    if(glyphInt >= (uint8_t) 'a' && glyphInt <= (uint8_t) 'z') {
        return (glyphInt - (uint8_t) 'a' + uppercase_offset);
    } 
    // It's a number
    if(glyphInt >= (uint8_t) '0' && glyphInt <= (uint8_t) '9') {
        return (glyphInt - (uint8_t) '0' + number_offset);
    }
    switch(glyph) {
        case 31:
            return 39;
        case 32:
            return 0;
        case 33:
            return 37;
        case 63:
            return 38;
        default:
            return 3;
    }
}

void render_text(uint8_t start_x, uint8_t start_y, char* text) {

    uint8_t x = start_x;
    uint8_t y = start_y;
    uint16_t index = 0;

    while(text[index]!='\0' && index < 16) {
        char glyph = text[index];
        uint8_t tile_index;
        tile_index = ascii_to_tile_pos(glyph);
        set_win_tiles(x, y, 1, 1, &Font_map[tile_index]);
        x++;
        index++;
    }
}