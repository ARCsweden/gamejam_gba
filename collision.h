#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>

typedef struct bounding_box_t {
    uint8_t right;
    uint8_t left;
    uint8_t top;
    uint8_t bottom;
} bounding_box_t;

bounding_box_t create_bb(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

// Returns 1 on collision, 0 on no overlap
uint8_t check_collision(bounding_box_t a, bounding_box_t b);

#endif
