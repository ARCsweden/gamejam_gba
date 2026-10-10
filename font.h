#ifndef FONT_H
#define FONT_H

#include "assets/Font.h"

uint8_t ascii_to_tile_pos(char);
void render_text(uint8_t x, uint8_t y, char* text);

#endif