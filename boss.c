#include "boss.h"
#include "gamestate.h"
#include "gamelogic.h"

Instruction instruction_list[MAX_INST];
uint8_t active_ins_index;
uint8_t append_ins_index; 
uint16_t progress;

void init_boss_logic(void) {
    progress = 0;
    active_ins_index = 0;
    append_ins_index = 0;
}

void update_boss_logic(void) {
    
    switch (instruction_list[active_ins_index].type) {
        case WAIT:
            break;
        case MOVE:
            //Move with speed
            if(instruction_list[active_ins_index].direction > 0) {
                state.boss.pos_x.w += instruction_list[active_ins_index].speed.w;
                if(state.boss.pos_x.h < border_left) state.boss.pos_x.h = border_left;
            }
            if(instruction_list[active_ins_index].direction < 0) {
                state.boss.pos_x.w -= instruction_list[active_ins_index].speed.w;
                if(state.boss.pos_x.h > border_right) state.boss.pos_x.h = border_right;
            }
            break;
        case SHOOT_SMPL:
            break;
        case SHOOT_LRG:
            break;
        case NEW_INS:
            //create_new_inst();
            break;
        default:
            break;
    }
    if(progress >= instruction_list[active_ins_index].duration) active_ins_index = (active_ins_index + 1) % MAX_INST;
    progress++;
}