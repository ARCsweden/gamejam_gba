#include <gb/gb.h>
#include <stdint.h>

#include "gamestate.h"

#include "audio.h"
#include "gfx.h"
#include "gamelogic.h"

void main(void)
{
    // Initialize graphics
    init_gfx();
    // Initialize audio
    init_audio();

    // Initialize game state
    struct gamestate_t state;
    state.oak_x.w = TO_FIXED(80, 0);
    state.oak_y.w = TO_FIXED(80, 0);
    state.letters_tile = 0;

    // Loop forever
    while(1) {
        state = update_game(state);

        update_gfx(state);

		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}

// For debugging, check out <gbdk/emu_debug.h>
// Can write messages, perform profiling, set breakpoints (requires compatible emulator)
