#ifndef AUDIO_H
#define AUDIO_H

#include "hUGEDriver.h"

// Note, reference to music.c, a track exported from hUGETracker
extern const hUGESong_t music;

void init_audio(void);

void play_song(hUGESong_t* track);

#endif
