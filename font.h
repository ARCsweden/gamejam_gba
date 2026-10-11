#ifndef FONT_H
#define FONT_H

#include "assets/Font.h"

#define FONT_PALETTE_DEFAULT 0
#define FONT_PALETTE_PLAYER 4
#define FONT_PALETTE_BOSS 5

uint8_t ascii_to_tile_pos(char);
void render_text(uint8_t x, uint8_t y, char* text);
void render_text_colored(uint8_t x, uint8_t y, char* text, uint8_t palette);

#endif