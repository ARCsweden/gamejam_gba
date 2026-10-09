#include "gamestate.h"

struct gamestate_t state;

void init_gamestate(void) {
    state.oak_x.w = TO_FIXED(80, 0);
    state.oak_y.w = TO_FIXED(80, 0);
    state.letters_tile = 0;
}
