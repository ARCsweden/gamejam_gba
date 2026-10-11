#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>

typedef struct bounding_box_t {
    uint8_t right;
    uint8_t left;
    uint8_t top;
    uint8_t bottom;
} bounding_box_t;

// Returns 1 on collision, 0 on no overlap
uint8_t check_collision(bounding_box_t a, bounding_box_t b);

#endif
