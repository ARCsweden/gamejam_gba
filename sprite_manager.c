#include "sprite_manager.h"
#include "art.h"
#include "gamestate.h"

// TODO: Struct for handling various buffers like projectiles
// TODO: Stuff for cleaning up sprites when they go out of range


struct projectile_t enemy_projectile_pool[NUM_SMALL_E_SPELLS];
struct projectile_t player_projectile_pool[NUM_PLAYER_SPELLS];

void init_sprite_manager(void) {
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        player_projectile_pool[i].alive = 0;
        player_projectile_pool[i].hw_sprite = P_SPELL_SPRITE + i;
    }
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        enemy_projectile_pool[i].alive = 0;
        enemy_projectile_pool[i].hw_sprite = S_SMALL_SPRITE + i;
    }
}

void _update_projectiles(struct projectile_t* pool, uint8_t pool_size) {
    for(uint8_t i = 0; i < pool_size; ++i) {
        if(pool[i].alive) {
            // Move in X
            if(pool[i].dir & 0x1) {
                pool[i].pos_x.w -= pool[i].vel_x.w;
            } else {
                pool[i].pos_x.w += pool[i].vel_x.w;
            }
            // Move in Y
            if(pool[i].dir & 0x2) {
                pool[i].pos_y.w -= pool[i].vel_y.w;
            } else {
                pool[i].pos_y.w += pool[i].vel_y.w;
            }

            // Check OOB
            if(
                pool[i].pos_x.h > SPRITE_LAYER_OOB_X ||
                pool[i].pos_y.h > SPRITE_LAYER_OOB_Y
            ) {
                pool[i].alive = 0;
                hide_sprite(pool[i].hw_sprite);
            }
        }
    }
}

void update_projectiles(void) {
    _update_projectiles(player_projectile_pool, NUM_PLAYER_SPELLS);
    _update_projectiles(enemy_projectile_pool, NUM_SMALL_E_SPELLS);
}

uint8_t spawn_player_projectile(uint8_t pos_x, uint8_t pos_y) {
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        if(player_projectile_pool[i].alive) continue;
        // This projectile is dead, reuse it
        player_projectile_pool[i].alive = 1;
        player_projectile_pool[i].pos_x.w = TO_FIXED(pos_x, 0);
        player_projectile_pool[i].pos_y.w = TO_FIXED(pos_y, 0);
        // TODO: Note, everything below can probably be constant set in init
        player_projectile_pool[i].dir = 0x2; // Negative y-dir
        player_projectile_pool[i].vel_x.w = 0; // Move straight ahead
        player_projectile_pool[i].vel_y.w = PLAYER_SPELL_SPEED;
        // TODO: Set collision rect
        player_projectile_pool[i].w = 2;
        player_projectile_pool[i].h = 8;
        return 1;
    }
    // Failed to create a projectile
    return 0;
}

void draw_projectiles(void) {
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        if(player_projectile_pool[i].alive) {
            move_metasprite_ex(p_spell_metasprites[0], p_spell_TILE_ORIGIN, 0, player_projectile_pool[i].hw_sprite, player_projectile_pool[i].pos_x.h,  player_projectile_pool[i].pos_y.h);
        }
    }
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        if(enemy_projectile_pool[i].alive) {
            // TODO: Dedicated metasprite
            move_metasprite_ex(p_spell_metasprites[0], p_spell_TILE_ORIGIN, 0, enemy_projectile_pool[i].hw_sprite, enemy_projectile_pool[i].pos_x.h,  enemy_projectile_pool[i].pos_y.h);
        }
    }
}
