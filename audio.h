#ifndef AUDIO_H
#define AUDIO_H

#include "hUGEDriver.h"

// Note, reference to music.c, a track exported from hUGETracker
extern const hUGESong_t music;

void init_audio(void);

uint8_t shoot_sfx();

void play_song(hUGESong_t* track);

#endif
