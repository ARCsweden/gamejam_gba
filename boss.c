#include "boss.h"
#include "gamestate.h"
#include "gamelogic.h"
#include "sprite_manager.h"

Instruction* instruction_list[MAX_INST];
uint8_t active_ins_index;
uint8_t append_ins_index; 
uint16_t progress;

void load_template(Instruction* template[]);

Instruction wait_1000ms = {WAIT, 0, 60};
Instruction wait_500ms = {WAIT, 0, 30};
Instruction skip = {WAIT, 0, 0};
Instruction move_right_small = {MOVE, 1, 30};
Instruction move_left_small = {MOVE, -1, 30};
Instruction new_inst = {NEW_INS, 0, 0};
Instruction shoot_simple = {SHOOT_SMPL, 0, 0};

Instruction* template1[7] = {&move_right_small,&wait_1000ms,&move_left_small,&shoot_simple,&wait_500ms,&shoot_simple,&new_inst};

void init_boss_logic(void) {
    progress = 0;
    active_ins_index = 0;
    append_ins_index = 0;
    move_right_small.speed.w = TO_FIXED(1,0);
    move_left_small.speed.w = TO_FIXED(1,0);
    for (int i = 0; i < MAX_INST; i++){
        instruction_list[i] = &skip;
    }
    load_template(template1);
}

void update_boss_logic(void) {
    fixed speed_x;
    fixed speed_y;
    speed_x.w = 0;
    speed_y.w = BOSS_SPELL_SPEED;
    if(state.boss.hp == 0){
        return;
    }
    switch (instruction_list[active_ins_index]->type) {
        case WAIT:
            break;
        case MOVE:
            //Move with speed
            if(instruction_list[active_ins_index]->direction > 0) {
                state.boss.pos_x.w += instruction_list[active_ins_index]->speed.w;
                if(state.boss.pos_x.h > border_right) state.boss.pos_x.h = border_right;
            }
            if(instruction_list[active_ins_index]->direction < 0) {
                state.boss.pos_x.w -= instruction_list[active_ins_index]->speed.w;
                if(state.boss.pos_x.h < border_left) state.boss.pos_x.h = border_left;
            }
            break;
        case SHOOT_SMPL:
            if(spawn_enemy_projectile(
            state.boss.pos_x.h,
            state.boss.pos_y.h + 8,
            0, // Straight down
            speed_x,
            speed_y
        ));
            break;
        case SHOOT_LRG:
            break;
        case NEW_INS:
            load_template(template1);
            break;
        default:
            break;
    }
    if(progress >= instruction_list[active_ins_index]->duration) {
        active_ins_index = (active_ins_index + 1) % MAX_INST;
        progress = 0;
    }
    else progress++;
}

void load_template(Instruction* template[]){
    uint8_t template_index = 0;
    while (template[template_index]->type != NEW_INS){
        instruction_list[append_ins_index] = template[template_index];
        append_ins_index = (append_ins_index + 1)%MAX_INST;
        template_index ++;
    }   
    instruction_list[append_ins_index] = &new_inst;
    append_ins_index = (append_ins_index + 1)%MAX_INST;

}

void update_boss_wave(void) {
    if(state.boss.dir & 0x2) {
        state.boss.vel_y.w += TO_FIXED(0, BOSS_ACC_Y);
        if(state.boss.vel_y.h == BOSS_PEAK_Y_SPEED) {
            state.boss.dir &= ~0x2;
        }
    } else {
        state.boss.vel_y.w -= TO_FIXED(0, BOSS_ACC_Y);
        if(state.boss.vel_y.w == 0) {
            state.boss.dir = ~state.boss.dir;
        }
    }

    if(state.boss.dir & 0x1) {
        state.boss.pos_y.w -= state.boss.vel_y.w;
    } else {
        state.boss.pos_y.w += state.boss.vel_y.w;
    }
}
