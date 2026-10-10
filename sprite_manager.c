#include "sprite_manager.h"

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
