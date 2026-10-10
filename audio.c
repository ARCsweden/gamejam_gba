#include "audio.h"
#include <gb/gb.h>



void play_song(hUGESong_t* track) {
    // Mute each channel
    hUGE_mute_channel(HT_CH1,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH2,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH3,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH4,HT_CH_MUTE);
    // Sample song is in bank 0
    // No bank switching needed, bank 0 is always active
    hUGE_init(track);
}

uint8_t shoot_sfx() {
    const uint8_t shoot_sfx_delay = 50;
    hUGE_mute_channel(HT_CH1,HT_CH_MUTE);
    NR10_REG=0X7D;
    NR11_REG=0XC1;
    NR12_REG=0X82;  // 92,82... for lower. B2,C2 etc... for higher
    NR13_REG=0XA6;
    NR14_REG=0X86;
    return shoot_sfx_delay;
}

void play_sfx() {

}

void init_audio(void) {
    NR52_REG = 0x80; // Master sound on
    NR50_REG = 0xFF; // Maximum volume for left/right speakers.
    NR51_REG = 0xFF; // Turn on sound fully
    // the critical tags ensure no interrupts will be called while this block of code is being executed
    __critical {
        // Init and use huge drive to play our sample song
        play_song(&music);
        add_VBL(hUGE_dosound);
    }
}
