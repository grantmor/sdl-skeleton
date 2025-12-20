#pragma once

#include "types.h"
#include <SDL3/SDL_audio.h>

#define NUM_SFX_CHANNELS 16

typedef struct {
	u32 id;
	u32 length;
	u8* data;
	char* path;
} SoundClip;

typedef struct {
	SDL_AudioStream* stream_pool[NUM_SFX_CHANNELS];
} SoundManager;
