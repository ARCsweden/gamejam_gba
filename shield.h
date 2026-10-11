#ifndef SHIELD_POWERUP_H
#define SHIELD_POWERUP_H

#include <stdint.h>
#include <gb/gb.h>

typedef struct shield_powerup_t {
    fixed pos_x;
    fixed pos_y;
    uint16_t cooldown;
    uint8_t spawned;
    uint8_t frame;
    uint8_t frame_timer;
} shield_powerup_t;

void spawn_shield_powerup(void);
void update_shield_powerup(void);
void pickup_shield(void);
void draw_shield_powerup(void);

#endif
