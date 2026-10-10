#include "gamelogic.h"

// Included to access state variable
#include "gamestate.h"
#include "sprite_manager.h"

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
        state.player.pos_x.w -= SPEED;
    }
    if((buttons & J_RIGHT)) {
        state.player.pos_x.w += SPEED;
    }
    if((buttons & J_UP)) {
        state.player.pos_y.w -= SPEED;
    }
    if((buttons & J_DOWN)) {
        state.player.pos_y.w += SPEED;
    }
    if((buttons & J_A) && state.player.spell_cooldown == 0) {
        // Do something on A pressed (once)
        if(spawn_player_projectile(state.player.pos_x.h, state.player.pos_y.h - 8)) {
            state.player.spell_cooldown = PLAYER_SPELL_COOLDOWN;
        }
    }
    buttons_prev = buttons;

    // Frame timer dependent animation
    if(state.player.spell_cooldown > 0) state.player.spell_cooldown--;

    frame++;
    if(frame >= 60) {
        frame = 0;
        //state.letters_tile++;
        //if(state.letters_tile >= 4) state.letters_tile = 0;
    }
    update_projectiles();
}

