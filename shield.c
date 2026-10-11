#include "shield.h"

#include "assets/shield_powerup.h"

#include "sprite_manager.h"
#include "gamestate.h"
#include <rand.h>

#define SHIELD_FRAME_TIMER 15
#define SHIELD_FRAMES 2

void spawn_shield_powerup(void) {
    uint8_t x_spawn = rand() % 128; // 0-127
    state.shield_powerup.spawned = 1;
    state.shield_powerup.pos_x.h = x_spawn + SPRITE_LAYER_ORIG_X + 16;
    state.shield_powerup.pos_y.w = 0;
}

void pickup_shield(void) {
    state.shield_powerup.spawned = 0;
    state.shield_powerup.cooldown = SHIELD_COOLDOWN;
    hide_sprite(SHIELD_POWERUP_SPRITE);
    hide_sprite(SHIELD_POWERUP_SPRITE + 1);

    state.player.shield = SHIELD_DURATION;
}

void update_shield_powerup(void) {
    if(state.shield_powerup.spawned) {
        // Animation
        state.shield_powerup.frame_timer++;
        if(state.shield_powerup.frame_timer >= SHIELD_FRAME_TIMER) {
            state.shield_powerup.frame_timer = 0;
            state.shield_powerup.frame++;
            if(state.shield_powerup.frame >= SHIELD_FRAMES) {
                state.shield_powerup.frame = 0;
            }
        }
        // Move
        state.shield_powerup.pos_y.w += SHIELD_MOVE_SPEED;

        // Check for OOB
        if(state.shield_powerup.pos_y.h > SPRITE_LAYER_OOB_Y) {
            state.shield_powerup.spawned = 0;
            state.shield_powerup.cooldown = SHIELD_COOLDOWN;
            hide_sprite(SHIELD_POWERUP_SPRITE);
            hide_sprite(SHIELD_POWERUP_SPRITE + 1);
        }
    } else if(state.player.shield) {
        state.player.shield--;
    } else {
        state.shield_powerup.cooldown--;
        if(state.shield_powerup.cooldown == 0) {
            spawn_shield_powerup();
        }
    }
}

void draw_shield_powerup(void) {
    if(state.shield_powerup.spawned) {
        move_metasprite_ex(shield_powerup_metasprites[state.shield_powerup.frame], shield_powerup_TILE_ORIGIN, 0, SHIELD_POWERUP_SPRITE, state.shield_powerup.pos_x.h, state.shield_powerup.pos_y.h);
    }
}
