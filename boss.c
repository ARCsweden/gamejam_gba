#include "boss.h"
#include "gamestate.h"
#include "gamelogic.h"
#include "sprite_manager.h"

Instruction* instruction_list[MAX_INST];
uint8_t active_ins_index;
uint8_t append_ins_index; 
uint16_t progress;

void load_template(Instruction* template[]);

const Instruction wait_1000ms = {WAIT, 0, 60};
const Instruction wait_500ms = {WAIT, 0, 30};
const Instruction wait_250ms = {WAIT, 0, 15};
const Instruction skip = {WAIT, 0, 0};
const Instruction move_right_small = {MOVE, 1, 30, {.w = TO_FIXED(1,0)}};
const Instruction move_left_small = {MOVE, -1, 30, {.w = TO_FIXED(1,0)}};
const Instruction new_inst = {NEW_INS, 0, 0};
const Instruction shoot_simple = {SHOOT_SMPL, 0, 0};

const Instruction s_s = {SHOOT_SMPL, 0, 0};
const Instruction s_s_l = {SHOOT_SMPL, 1,0, {.w = TO_FIXED(0,48)}};
const Instruction s_s_r = {SHOOT_SMPL, 0,0, {.w = TO_FIXED(0,48)}};


const Instruction mv_r_1 = {MOVE, 1, 4, {.w = TO_FIXED(2,0)}};
const Instruction mv_l_1 = {MOVE, -1, 4, {.w = TO_FIXED(2,0)}};
const Instruction mv_r_2 = {MOVE, 1, 8, {.w = TO_FIXED(2,0)}};
const Instruction mv_l_2 = {MOVE, -1, 8, {.w = TO_FIXED(2,0)}};
const Instruction mv_r_3 = {MOVE, 1, 12, {.w = TO_FIXED(2,0)}};
const Instruction mv_l_3 = {MOVE, -1, 12, {.w = TO_FIXED(2,0)}};
const Instruction mv_r_4 = {MOVE, 1, 16, {.w = TO_FIXED(2,0)}};
const Instruction mv_l_4 = {MOVE, -1, 16, {.w = TO_FIXED(2,0)}};

const Instruction mv_r_1s = {MOVE, 1, 8, {.w = TO_FIXED(1,0)}};
const Instruction mv_l_1s = {MOVE, -1, 8, {.w = TO_FIXED(1,0)}};
const Instruction mv_r_2s = {MOVE, 1, 16, {.w = TO_FIXED(1,0)}};
const Instruction mv_l_2s = {MOVE, -1, 16, {.w = TO_FIXED(1,0)}};
const Instruction mv_r_3s = {MOVE, 1, 24, {.w = TO_FIXED(1,0)}};
const Instruction mv_l_3s = {MOVE, -1, 24, {.w = TO_FIXED(1,0)}};
const Instruction mv_r_4s = {MOVE, 1, 32, {.w = TO_FIXED(1,0)}};
const Instruction mv_l_4s = {MOVE, -1, 32, {.w = TO_FIXED(1,0)}};

const Instruction* template1[7] = {&move_right_small,&wait_1000ms,&move_left_small,&shoot_simple,&wait_500ms,&shoot_simple,&new_inst};
const //Instruction* template1[7] = {&move_right_small,&wait_1000ms,&move_left_small,&shoot_simple,&wait_500ms,&shoot_simple,&new_inst};
const Instruction* template2[10] = {&mv_l_1,&shoot_simple,&mv_l_1,&shoot_simple,&mv_l_1,&shoot_simple,&mv_r_2,&shoot_simple,&mv_r_1,&new_inst};

const Instruction* easy_left[13] = {&mv_l_4,&mv_l_4,&s_s_l,&mv_r_1,&s_s_l,&mv_r_1,&s_s_l,&mv_r_1,&s_s_l,&mv_r_1,&mv_r_4,&new_inst};
const Instruction* easy_right[13] = {&mv_r_4,&mv_r_4,&s_s_r,&mv_l_1,&s_s_r,&mv_l_1,&s_s_r,&mv_l_1,&s_s_r,&mv_l_1,&mv_l_4,&new_inst};
const Instruction* mid_left[10] = {&mv_l_1,&s_s,&wait_250ms,&s_s_l,&wait_250ms,&s_s_r,&wait_250ms,&mv_r_1s,&wait_500ms,&new_inst};
const Instruction* mid_right[10] = {&mv_r_1,&s_s,&wait_250ms,&s_s_l,&wait_250ms,&s_s_r,&wait_250ms,&mv_l_1s,&wait_500ms,&new_inst};
void init_boss_logic(void) {
    progress = 0;
    active_ins_index = 0;
    append_ins_index = 0;
    //move_right_small.speed.w = TO_FIXED(1,0);
    for (int i = 0; i < MAX_INST; i++){
        instruction_list[i] = &skip;
    }
    load_template(easy_right);
}

void update_boss_logic(void) {
    fixed speed_x;
    fixed speed_y;
    speed_x.w = TO_FIXED(0,0);
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
                instruction_list[active_ins_index]->direction, // Straight down
                instruction_list[active_ins_index]->speed,
                speed_y
            ));
            break;
        case SHOOT_LRG:
            break;
        case NEW_INS:
        if (state.player.pos_x.h > 50 && state.player.pos_x.h < 110){
            if(state.player.pos_x.h > 80){
            load_template(mid_right);
            }
            else{
                load_template(mid_left);
            }
        }
        else if(state.player.pos_x.h > 80){
            load_template(easy_right);

        }
            else{
                load_template(easy_left);
            }
            break;
        default:
            break;
    }
    if(progress + 1 >= instruction_list[active_ins_index]->duration) {
        active_ins_index = (active_ins_index + 1) % MAX_INST;
        progress = 0;
    }
    else {progress++;}
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
