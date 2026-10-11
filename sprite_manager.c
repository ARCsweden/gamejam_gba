#include "sprite_manager.h"
#include "art.h"
#include "gamestate.h"

// TODO: Struct for handling various buffers like projectiles
// TODO: Stuff for cleaning up sprites when they go out of range


projectile_t enemy_projectile_pool[NUM_SMALL_E_SPELLS];
projectile_t player_projectile_pool[NUM_PLAYER_SPELLS];

uint8_t player_projectile_alive = 0;
uint8_t enemy_projectile_alive = 0;

void init_sprite_manager(void) {
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        player_projectile_alive &= ~(1 << i);
        player_projectile_pool[i].hw_sprite = P_SPELL_SPRITE + i;
    }
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        enemy_projectile_alive &= ~(1 << i);
        enemy_projectile_pool[i].hw_sprite = S_SMALL_SPRITE + i;
    }
}

void _update_projectiles(projectile_t* pool, uint8_t* pool_alive, uint8_t pool_size) {
    uint8_t alive = *pool_alive;
    for(uint8_t i = 0; i < pool_size; ++i) {
        if(alive & 0x1) {
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
                *pool_alive &= ~(1 << i);
                hide_sprite(pool[i].hw_sprite);
            }
        }
        alive >>= 1;
    }
}

void update_projectiles(void) {
    _update_projectiles(player_projectile_pool, &player_projectile_alive, NUM_PLAYER_SPELLS);
    _update_projectiles(enemy_projectile_pool, &enemy_projectile_alive, NUM_SMALL_E_SPELLS);
}

uint8_t _spawn_projectile(projectile_t* pool, uint8_t* pool_alive, uint8_t pool_size, uint8_t pos_x, uint8_t pos_y, uint8_t dir, fixed vel_x, fixed vel_y, uint8_t col_w, uint8_t col_h) {
    uint8_t alive = *pool_alive;
    for(uint8_t i = 0; i < pool_size; ++i) {
        if((alive & 0x1) == 0) {
            // This projectile is dead, reuse it
            *pool_alive |= (1 << i);
            pool[i].pos_x.w = TO_FIXED(pos_x, 0);
            pool[i].pos_y.w = TO_FIXED(pos_y, 0);
            pool[i].dir = dir;
            pool[i].vel_x = vel_x;
            pool[i].vel_y = vel_y;
            pool[i].w = col_w;
            pool[i].h = col_h;
            return 1;
        }
        alive >>= 1;
    }
    // Failed to create a projectile
    return 0;
}

uint8_t spawn_player_projectile(void) {
    fixed speed_x;
    fixed speed_y;
    speed_x.w = 0;
    speed_y.w = PLAYER_SPELL_SPEED;
    return _spawn_projectile(
        player_projectile_pool,
        &player_projectile_alive,
        NUM_PLAYER_SPELLS,
        state.player.pos_x.h,
        state.player.pos_y.h - 8,
        0x2, // Forward dir
        speed_x, // Straight ahead
        speed_y,
        // TODO: Coll
        4,
        8
    );
}

uint8_t spawn_enemy_projectile(uint8_t pos_x, uint8_t pos_y, uint8_t dir, fixed vel_x, fixed vel_y) {
    return _spawn_projectile(
        enemy_projectile_pool,
        &enemy_projectile_alive,
        NUM_SMALL_E_SPELLS,
        pos_x,
        pos_y,
        dir,
        vel_x,
        vel_y,
        // TODO: Coll
        4,
        8
    );
}

void draw_projectiles(void) {
    uint8_t alive = player_projectile_alive;
    for(uint8_t i = 0; i < NUM_PLAYER_SPELLS; ++i) {
        if(alive & 0x1) {
            move_metasprite_ex(p_spell_metasprites[0], p_spell_TILE_ORIGIN, 0, player_projectile_pool[i].hw_sprite, player_projectile_pool[i].pos_x.h,  player_projectile_pool[i].pos_y.h);
        }
        alive >>= 1;
    }
    alive = enemy_projectile_alive;
    for(uint8_t i = 0; i < NUM_SMALL_E_SPELLS; ++i) {
        if(alive & 0x1) {
            // TODO: Dedicated metasprite
            move_metasprite_ex(p_spell_metasprites[1], p_spell_TILE_ORIGIN, 0, enemy_projectile_pool[i].hw_sprite, enemy_projectile_pool[i].pos_x.h,  enemy_projectile_pool[i].pos_y.h);
        }
        alive >>= 1;
    }
}
