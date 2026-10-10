#ifndef BOSS_H
#define BOSS_H

#include <stdint.h>
#include <gb/gb.h>

#define MAX_INST 16
#define BOSS_MAXHP 4 

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
    uint16_t duration;
    fixed speed; 
} Instruction;

typedef struct boss_t {
    fixed pos_x;
    fixed pos_y;
    fixed vel_y;
    uint8_t dir;
    uint8_t spell_cooldown;
    uint8_t health;
}boss_t;

void init_boss_logic(void);

void update_boss_logic(void);

void update_boss_wave(void);

#endif