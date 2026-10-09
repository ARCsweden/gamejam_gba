#include <gb/gb.h>
#include <stdint.h>

#include "audio.h"
#include "gfx.h"
#include "gamelogic.h"
#include "gamestate.h"

void main(void)
{
    // Initialize graphics
    init_gfx();
    // Initialize audio
    init_audio();
    // Set the initial variables
    init_gamestate();

    // Loop forever
    while(1) {
        // Read input and update ingame logic
        update_game();

        // Render graphics
        update_gfx();

		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}

// For debugging, check out <gbdk/emu_debug.h>
// Can write messages, perform profiling, set breakpoints (requires compatible emulator)
