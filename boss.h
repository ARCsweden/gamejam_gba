#ifndef BOSS_H
#define BOSS_H

#include <stdint.h>
#include <gb/gb.h>

#define MAX_INST 16

typedef enum ins_type {
    WAIT = 0,
    MOVE = 1,
    SHOOT_SMPL = 2,
    SHOOT_LRG = 3,
    NEW_INS = 4
}ins_type;

typedef struct instruction {
    ins_type type;
    int8_t direction;
    fixed speed; 
    uint16_t duration;
} Instruction;

typedef struct boss_t {
    fixed pos_x;
    fixed pos_y;
    uint8_t spell_cooldown;
}boss_t;

void init_boss_logic(void);

void update_boss_logic(void);


#endif