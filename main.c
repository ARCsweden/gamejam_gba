#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>

#include "audio.h"
#include "gfx.h"
#include "gamelogic.h"
#include "gamestate.h"
#include "boss.h"
#include "intro.h"

#define SLOWDOWN_FRAMES 2
uint8_t slowdown_frame = 0;

void main(void)
{
    // Note, this doesn't appear to work on hardware, so keep it disabled
    //cpu_fast();

    // Initialize graphics
    init_gfx();
    // Initialize audio
    init_audio();
    // Set the initial variables
    init_gamestate();
    // set initial boss logic
    init_boss_logic();

    run_intro();

    // Loop forever
    while(1) {
        // Read input and update ingame logic (note slowdown effect when you've won/lost)
        if((end_state == 0) || (slowdown_frame == 0)) {
            slowdown_frame = SLOWDOWN_FRAMES;
            update_game();
        } else {
            slowdown_frame--;
        }
		// Done processing, yield CPU and wait for start of next frame
        vsync();
        // Render graphics
        update_gfx();
    }
}

// For debugging, check out <gbdk/emu_debug.h>
// Can write messages, perform profiling, set breakpoints (requires compatible emulator)
