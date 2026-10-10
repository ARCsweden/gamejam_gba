#include "gamelogic.h"
#include <gb/gb.h>

// Included to access state variable
#include "gamestate.h"
#include "sprite_manager.h"
#include "collision.h"

// Include audio
#include "audio.h"

// Audio vars
uint8_t sound_ch1_counter = 0;
uint8_t sound_ch2_counter = 0;
uint8_t sound_ch3_counter = 0;
uint8_t sound_ch4_counter = 0;


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
    uint8_t alive = state.player.hp > 0;
    if(alive) {
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
        // Shoot spell when holding A
        if((buttons & J_A) && state.player.spell_cooldown == 0) {
            // Do something on A pressed (once)
            if(spawn_player_projectile()) {
                state.player.spell_cooldown = PLAYER_SPELL_COOLDOWN;
                sound_ch1_counter = shoot_sfx();
            }
        }
        // Activate shield when holding B
        state.player.shield = (buttons & J_B);
    } else if(state.player.dead_pos_y.h <= SPRITE_LAYER_OOB_Y) {
        state.player.dead_pos_y.w += SPEED_DOWN;
    }

    buttons_prev = buttons;

    if(state.boss.hp > 0) {
        update_boss_wave();
        update_boss_logic();
    } else {
        if(state.boss.dead_pos_y.h <= SPRITE_LAYER_OOB_Y) {
            state.boss.dead_pos_y.w += SPEED_DOWN;
        }
    }

    // Frame timer dependent animation
    if(state.player.spell_cooldown > 0) state.player.spell_cooldown--;
    if(state.boss.spell_cooldown > 0) state.boss.spell_cooldown--;

    // Sound Handler
    // Counters for each sound channel to resume playing music after a SFX
    if(sound_ch1_counter > 0) {
        sound_ch1_counter--;
        if(sound_ch1_counter == 0) {
            hUGE_mute_channel(HT_CH1,HT_CH_PLAY);
        }
    }
    if(sound_ch2_counter > 0) {
        sound_ch2_counter--;
        if(sound_ch2_counter == 0) {
            hUGE_mute_channel(HT_CH2,HT_CH_PLAY);
        }
    }
    if(sound_ch3_counter > 0) {
        sound_ch3_counter--;
        if(sound_ch3_counter == 0) {
            hUGE_mute_channel(HT_CH3,HT_CH_PLAY);
        }
    }
    if(sound_ch4_counter > 0) {
        sound_ch4_counter--;
        if(sound_ch4_counter == 0) {
            hUGE_mute_channel(HT_CH4,HT_CH_PLAY);
        }
    }


    frame++;
    if(frame >= 10) {
        frame = 0;
        state.player.anim_frame++;
        if(state.player.anim_frame >= 4) state.player.anim_frame = 0;
    }
    update_projectiles();

    // Check collisions
    bounding_box_t player_bb = create_bb(state.player.pos_x.h - 4, state.player.pos_y.h - 4, 8, 8);
    bounding_box_t boss_bb = create_bb(state.boss.pos_x.h - 6, state.boss.pos_y.h - 6, 12, 12);

    // Check collision between boss and spells
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        projectile_t proj = player_projectile_pool[i];
        if(proj.alive) {
            bounding_box_t spell_bb = create_bb(proj.pos_x.h - proj.w / 2, proj.pos_y.h - proj.h / 2, proj.w, proj.h);
            if(check_collision(boss_bb, spell_bb)) {
                player_projectile_pool[i].alive = 0;
                hide_sprite(player_projectile_pool[i].hw_sprite);

                if(state.boss.hp > 0) {
                    state.boss.hp--;
                    if (state.boss.hp == 0) {
                        // TODO: play vfx and remove boss wizard
                        state.boss.dead_pos_y = state.boss.pos_y;
                    }
                }
            }
        }
    }
    // Check collision between player and boss spells
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        projectile_t proj = enemy_projectile_pool[i];
        if(proj.alive) {
            bounding_box_t spell_bb = create_bb(proj.pos_x.h - proj.w / 2, proj.pos_y.h - proj.h / 2, proj.w, proj.h);
            if(check_collision(player_bb, spell_bb)) {
                enemy_projectile_pool[i].alive = 0;
                hide_sprite(enemy_projectile_pool[i].hw_sprite);
                // TODO: Boss hits player logic
                if(state.player.hp > 0) {
                    state.player.hp--;
                    if(state.player.hp == 0) {
                        state.player.dead_pos_y = state.player.pos_y;
                    }
                }
            }
        }
    }
}

