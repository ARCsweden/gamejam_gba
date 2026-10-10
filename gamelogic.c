#include "gamelogic.h"
#include <gb/gb.h>

// Included to access state variable
#include "gamestate.h"
#include "sprite_manager.h"

// Holding buttons pressed
uint8_t buttons = 0;
uint8_t buttons_prev = 0;

const uint8_t border_left = SPRITE_LAYER_ORIG_X + 8;
const uint8_t border_right = SPRITE_LAYER_SCREEN_LIM_X - 8;
const uint8_t border_bottom = SPRITE_LAYER_SCREEN_LIM_Y - 16;
const uint8_t border_top = SPRITE_LAYER_ORIG_Y + 72;

uint8_t frame = 0;

void update_game(void) {
    // Input
    // J_START, J_SELECT, J_A, J_B, J_UP, J_DOWN, J_LEFT, J_RIGHT
    buttons = joypad();
    // Checking for input goes here ...
    if((buttons & J_LEFT)) {
        state.player.pos_x.w -= SPEED_SIDEWAYS;
        if(state.player.pos_x.h < border_left) state.player.pos_x.h = border_left;
    }
    if((buttons & J_RIGHT)) {
        state.player.pos_x.w += SPEED_SIDEWAYS;
        if(state.player.pos_x.h > border_right) state.player.pos_x.h = border_right;
    }
    if((buttons & J_UP)) {
        state.player.pos_y.w -= SPEED_UP;
        if(state.player.pos_y.h < border_top) state.player.pos_y.h = border_top;
    }
    if((buttons & J_DOWN)) {
        state.player.pos_y.w += SPEED_DOWN;
        if(state.player.pos_y.h > border_bottom) state.player.pos_y.h = border_bottom;
    }
    if((buttons & J_A) && state.player.spell_cooldown == 0) {
        // Do something on A pressed (once)
        if(spawn_player_projectile()) {
            state.player.spell_cooldown = PLAYER_SPELL_COOLDOWN;
        }
    }
    if((buttons & J_B) && state.boss.spell_cooldown == 0) {
        fixed speed_x;
        fixed speed_y;
        speed_x.w = 0;
        speed_y.w = BOSS_SPELL_SPEED;
        // Do something on A pressed (once)
        if(spawn_enemy_projectile(
            state.boss.pos_x.h,
            state.boss.pos_y.h + 8,
            0, // Straight down
            speed_x,
            speed_y
        )) {
            state.boss.spell_cooldown = BOSS_SPELL_COOLDOWN;
        }
    }

    buttons_prev = buttons;

    update_boss_logic();

    // Frame timer dependent animation
    if(state.player.spell_cooldown > 0) state.player.spell_cooldown--;
    if(state.boss.spell_cooldown > 0) state.boss.spell_cooldown--;

    frame++;
    if(frame >= 10) {
        frame = 0;
        state.player.anim_frame++;
        if(state.player.anim_frame >= 4) state.player.anim_frame = 0;
    }
    update_projectiles();
}

