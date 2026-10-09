#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

// HW Sprite indices (each 2 sprites big)
// ======================================
#define NUM_HW_SPRITES 40

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
#define NUM_LARGE_E_SPELLS 4
// Player spells (1 wide)
#define P_SPELL_SPRITE (S_LARGE_SPRITE + NUM_LARGE_E_SPELLS * 2)
#define NUM_PLAYER_SPELLS 4
// Enemy small spells (1 wide)
#define S_SMALL_SPRITE (P_SPELL_SPRITE + NUM_PLAYER_SPELLS)
#define NUM_SMALL_E_SPELLS (NUM_HW_SPRITES - S_SMALL_SPRITE)

// TODO: Effects etc.

void init_sprite_manager(void);

#endif
