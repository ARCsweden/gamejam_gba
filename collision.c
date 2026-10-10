#include "collision.h"

bounding_box_t create_bb(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    bounding_box_t box = {
        .right = x + w,
        .left = x,
        .top = y,
        .bottom = y + h
    };
    return box;
}

// Returns 1 on collision, 0 on no overlap
uint8_t check_collision(bounding_box_t a, bounding_box_t b) {
    // Check if we are outside of the bounding boxes (no overlap possible)
    if(a.right < b.left) {
        return 0;
    }
    if(a.left >= b.right) {
        return 0;
    }
    if(a.top >= b.bottom) {
        return 0;
    }
    if(a.bottom < b.top) {
        return 0;
    }
    // If all these fail, there is an overlap/collision
    return 1;
}
