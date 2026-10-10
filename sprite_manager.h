#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <stdint.h>
#include <gb/gb.h>

// HW Sprite indices (each 2 sprites big)
// ======================================
// The lowest indice has the highest priority in terms of what is drawn

// Player sprites (2 wide)
#define P_WIZARD_SPRITE 0
#define P_CARPET_SPRITE 2
#define P_SHIELD_SPRITE 4
// Enemy sprites (2 wide)
#define E_WIZARD_SPRITE 6
#define E_CARPET_SPRITE 8
// Enemy large spells (2 wide)
#define S_LARGE_SPRITE 10
// NUMBER OF ENEMIES ON SCREEN AT THE SAME TIME
#define NUM_LARGE_E_SPELLS 4
#define LARGE_E_SPELL_SIZE 2
// Player spells (1 wide)
#define P_SPELL_SPRITE (S_LARGE_SPRITE + NUM_LARGE_E_SPELLS * LARGE_E_SPELL_SIZE)
// NUMBER OF PLAYER PROJECTILES ON SCREEN AT THE SAME TIME
#define NUM_PLAYER_SPELLS 4
#define PLAYER_SPELL_SIZE 1
// Enemy small spells (1 wide)
#define S_SMALL_SPRITE (P_SPELL_SPRITE + NUM_PLAYER_SPELLS)
// Note, MAX_HARDWARE_SPRITES is from gb.h, 40
// NUMBER OF ENEMY PROJECTILES ON SCREEN AT THE SAME TIME
#define NUM_SMALL_E_SPELLS (MAX_HARDWARE_SPRITES - S_SMALL_SPRITE)


// Logic for sprite plane bounding box (off screen region)

// Top/left corner of the screen
#define SPRITE_LAYER_ORIG_X 8
#define SPRITE_LAYER_ORIG_Y 16
// Bottom/right corner of the screen
#define SPRITE_LAYER_SCREEN_LIM_X (SPRITE_LAYER_ORIG_X + 160)
#define SPRITE_LAYER_SCREEN_LIM_Y (SPRITE_LAYER_ORIG_Y + 144)
// Out-of-bounds; we will despawn things outside this area
#define SPRITE_LAYER_OOB_X (SPRITE_LAYER_SCREEN_LIM_X + 16)
#define SPRITE_LAYER_OOB_Y (SPRITE_LAYER_SCREEN_LIM_Y + 16)

struct projectile_t {
    uint8_t pos_x;
    uint8_t pos_y;
    // Collision, centered around pos_x
    uint8_t w;
    uint8_t h;
    // Flags
    uint8_t alive;
    // Rendering
    uint8_t hw_sprite; // Note, first HW sprite
    uint8_t metasprite;
};

extern struct projectile_t enemy_projectile_pool[NUM_SMALL_E_SPELLS];
extern struct projectile_t player_projectile_pool[NUM_PLAYER_SPELLS];


// TODO: Effects etc.

void init_sprite_manager(void);

#endif
