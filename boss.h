#ifndef BOSS_H
#define BOSS_H

#include <stdint.h>
#include <gb/gb.h>

#define MAX_INST 16
#define BOSS_MAXHP 10 

typedef enum ins_type {
    WAIT = 0,
    MOVE = 1,
    SHOOT_SMPL = 2,
    SHOOT_SMPL_L = 3,
    SHOOT_SMPL_R = 4,
    SHOOT_LRG = 5,
    NEW_INS = 6
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
    fixed dead_pos_y;
    fixed vel_y;
    uint8_t dir;
    uint8_t spell_cooldown;
    uint8_t hp;
    uint8_t hit_timer;
}boss_t;

void init_boss_logic(void);

void update_boss_logic(void);

void update_boss_wave(void);

#endif