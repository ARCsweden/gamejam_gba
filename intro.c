#include "intro.h"
#include "font.h"
#include "gfx.h"


void run_intro(void){



    update_gfx();


    char text[12] = "PRESS START";
    render_text(8, 0, text);
    waitpad(J_START);

    render_text_colored(0, 0, "YOU TOOK MY CARPET!", FONT_PALETTE_PLAYER);
    waitpad(J_A);
    waitpadup();

    render_text_colored(0, 0, "I TAKE WHAT I WANT!", FONT_PALETTE_BOSS);
    waitpad(J_A);
    waitpadup();

    render_text_colored(0, 0, "I TAKE YOUR LIFE!", FONT_PALETTE_PLAYER);
    waitpad(J_A);
    waitpadup();
/*
    render_text(1, 0, "");
    waitpad(J_A);
*/


    render_text(0, 0, " ");
}
    