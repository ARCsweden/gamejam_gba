#include "gamelogic.h"

// Included to access state variable
#include "gamestate.h"

// Holding buttons pressed
uint8_t buttons = 0;
uint8_t buttons_prev = 0;

uint8_t frame = 0;

void update_game(void) {
    // Input
    // J_START, J_SELECT, J_A, J_B, J_UP, J_DOWN, J_LEFT, J_RIGHT
    buttons = joypad();
    // Checking for input goes here ...
    if((buttons & J_LEFT)) {
        state.player.pos_x.w -= SPEED_SIDEWAYS;
    }
    if((buttons & J_RIGHT)) {
        state.player.pos_x.w += SPEED_SIDEWAYS;
    }
    if((buttons & J_UP)) {
        state.player.pos_y.w -= SPEED_UP;
    }
    if((buttons & J_DOWN)) {
        state.player.pos_y.w += SPEED_DOWN;
    }
    if((buttons & J_A) && (~buttons_prev & J_A)) {
        // Do something on A pressed (once)
    }
    buttons_prev = buttons;

    // Frame timer dependent animation
    frame++;
    if(frame >= 60) {
        frame = 0;
        //state.letters_tile++;
        //if(state.letters_tile >= 4) state.letters_tile = 0;
    }
}

