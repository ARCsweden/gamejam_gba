#include "gamelogic.h"
#include <gb/gb.h>

// Included to access state variable
#include "gamestate.h"
#include "sprite_manager.h"
#include "collision.h"
#include "art.h"

// Include audio
#include "audio.h"


// Audio vars
uint8_t sound_ch1_counter = 0;
uint8_t sound_ch2_counter = 0;
uint8_t sound_ch3_counter = 0;
uint8_t sound_ch4_counter = 0;

// Font vars
uint8_t color1_counter = 120;
uint8_t color2_counter = 120;
uint8_t color3_counter = 120;

int8_t color1_delta = 6;
int8_t color2_delta = 6;
int8_t color3_delta = 6;

// Holding buttons pressed
uint8_t buttons = 0;
uint8_t buttons_prev = 0;

const uint8_t border_left = SPRITE_LAYER_ORIG_X + 8;
const uint8_t border_right = SPRITE_LAYER_SCREEN_LIM_X - 8;
const uint8_t border_bottom = SPRITE_LAYER_SCREEN_LIM_Y - 16;
const uint8_t border_top = SPRITE_LAYER_ORIG_Y + 72;

uint8_t frame = 0;

// Keep an array of bounding boxes for collision rather than creating them each frame
#define BB_PLAYER 0
#define BB_BOSS 1
#define BB_SPELL 2
#define BB_SHIELD 3
#define BB_SHIELD_POWERUP 4
static bounding_box_t _bbs[5];

void _update_bb(uint8_t index, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    bounding_box_t* bb = &_bbs[index];
    bb->right = x + w;
    bb->left = x;
    bb->top = y;
    bb->bottom = y + h;
}

void handle_input(void) {
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
    } else if(state.player.dead_pos_y.h <= SPRITE_LAYER_OOB_Y) {
        state.player.dead_pos_y.w += SPEED_DOWN;
    } else {
        waitpad(J_START);
        reset();
    }
    // Note, not used atm
    buttons_prev = buttons;
}

palette_color_t ui_palette[4] = {
    RGB8(255,255,255), RGB8(0, 0, 0), RGB8(255, 0, 0), RGB8(  100,  10,  255)
};

void update_ui_palette(void) {
    if(color2_counter > (250-color2_delta)) {
        color2_delta = color2_delta * -1;
        color2_counter = 250-color2_delta;
    }
    if(color2_counter < (10+color2_delta)) {
        color2_delta = color2_delta * -1; 
        color2_counter = 10+color2_delta;
    }

    if(color3_counter > (250-color3_delta)) {
        color3_delta = color3_delta * -1;
        color3_counter = 250-color3_delta;
    }
    if(color3_counter < (10+color3_delta)) {
        color3_delta = color3_delta * -1; 
        color3_counter = 10+color3_delta;
    }
    color2_counter = color2_counter + color2_delta;
    color3_counter = color3_counter + color3_delta;

    ui_palette[2] = RGB8(255, color2_counter, color3_counter);

    set_bkg_palette(S_PAL(0), 1, ui_palette);
}

void update_sound(void) {
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
}

void update_frame_animation(void) {
    frame++;
    if(frame >= 10) {
        frame = 0;
        state.player.anim_frame++;
        if(state.player.anim_frame >= 4) state.player.anim_frame = 0;
    }
    // Hit FX
    if(state.player.hit_timer > 0) {
        state.player.hit_timer--;
        if(state.player.hit_timer == 0) {
            set_sprite_palette(S_PAL(1), 1, wizard_palettes);
        }
    }
    if(state.boss.hit_timer > 0) {
        state.boss.hit_timer--;
        if(state.boss.hit_timer == 0) {
            set_sprite_palette(S_PAL(4), 1, boss_palettes);
        }
    }
}

void update_collisions(void) {
    // Check collisions (last two parameters define the size of the hitbox, w/h)
    _update_bb(BB_PLAYER, state.player.pos_x.h - 4, state.player.pos_y.h - 4, 8, 8);
    if(state.boss.hp) {
        _update_bb(BB_BOSS, state.boss.pos_x.h - 6, state.boss.pos_y.h - 6, 12, 12);
    } else {
        _update_bb(BB_BOSS, state.boss.pos_x.h - 6, state.boss.dead_pos_y.h - 6, 12, 12);
    }
    // Player shield
    if(state.player.shield) {
        _update_bb(BB_SHIELD, state.player.pos_x.h - 6, state.player.pos_y.h - 20, 12, 8);
    }

    // Ragebait check
    if(state.player.hp > 0 && check_collision(&_bbs[BB_BOSS], &_bbs[BB_PLAYER])) {
        state.player.hp = 0;
        state.player.dead_pos_y = state.player.pos_y;
    }

    // Check collision between boss and spells
    uint8_t alive = player_projectile_alive;
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        if(alive & 0x1) {
            projectile_t* proj = &player_projectile_pool[i];
            _update_bb(BB_SPELL, proj->pos_x.h - proj->w / 2, proj->pos_y.h - proj->h / 2, proj->w, proj->h);
            if(check_collision(&_bbs[BB_BOSS], &_bbs[BB_SPELL])) {
                player_projectile_alive &= ~(1 << i);
                hide_sprite(player_projectile_pool[i].hw_sprite);
                state.boss.hit_timer = HIT_FX_FRAMES;
                set_sprite_palette(S_PAL(4), 1, boss_hit_palettes);
                // Check if boss is alive
                if(state.boss.hp > 0) {
                    state.boss.hp--;
                    if (state.boss.hp == 0) {
                        // TODO: play vfx and remove boss wizard
                        state.boss.dead_pos_y = state.boss.pos_y;
                    }
                }
            }
        }
        alive >>= 1;
    }
    // Check collision between player and boss spells
    alive = enemy_projectile_alive;
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        if(alive & 0x1) {
            projectile_t* proj = &enemy_projectile_pool[i];
            _update_bb(BB_SPELL, proj->pos_x.h - proj->w / 2, proj->pos_y.h - proj->h / 2, proj->w, proj->h);
            if(state.player.shield && check_collision(&_bbs[BB_SHIELD], &_bbs[BB_SPELL])) {
                enemy_projectile_alive &= ~(1 << i);
                hide_sprite(enemy_projectile_pool[i].hw_sprite);
            }
            else if(check_collision(&_bbs[BB_PLAYER], &_bbs[BB_SPELL])) {
                enemy_projectile_alive &= ~(1 << i);
                hide_sprite(enemy_projectile_pool[i].hw_sprite);
                state.player.hit_timer = HIT_FX_FRAMES;
                set_sprite_palette(S_PAL(1), 1, boss_palettes);
                // TODO: Boss hits player logic
                if(state.player.hp > 0) {
                    state.player.hp--;
                    if(state.player.hp == 0) {
                        state.player.dead_pos_y = state.player.pos_y;
                    }
                }
            }
        }
        alive >>= 1;
    }
    // Check player picking up shield powerup
    if(state.shield_powerup.spawned && state.player.hp > 0) {
        _update_bb(BB_SHIELD_POWERUP, state.shield_powerup.pos_x.h - 5, state.shield_powerup.pos_y.h - 5, 10, 10);
        if(check_collision(&_bbs[BB_PLAYER], &_bbs[BB_SHIELD_POWERUP])) {
            pickup_shield();
        }
    }
}

void update_game(void) {
    // Input
    handle_input();

    // Run boss logic
    if(state.boss.hp > 0) {
        update_boss_wave();
        update_boss_logic();
    } else {
        if(state.boss.dead_pos_y.h <= SPRITE_LAYER_OOB_Y) {
            state.boss.dead_pos_y.w += SPEED_DOWN;
        } else {
            waitpad(J_START);
            reset();
        }
    }

    // Frame timer dependent animation
    if(state.player.spell_cooldown > 0) state.player.spell_cooldown--;
    if(state.boss.spell_cooldown > 0) state.boss.spell_cooldown--;

    // Shifting palette over time
    update_ui_palette();

    // Sound Handler
    update_sound();

    // animate player, boss and hit FX
    update_frame_animation();

    // Move entities around
    update_projectiles();
    update_shield_powerup();

    update_collisions();
}

