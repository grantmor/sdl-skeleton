#pragma once

#include <SDL3/SDL_audio.h>
#include "types.h"

#define NUM_SFX_CHANNELS 64
#define MAX_SOUNDS 16

typedef struct {
	u32 id;
	u32 length;
	u8* data;
	char* path;
} SoundClip;

typedef struct {
	SDL_AudioStream* stream_pool[NUM_SFX_CHANNELS];
	SDL_AudioDeviceID device_id;
	SoundClip clips[MAX_SOUNDS];
	SDL_AudioSpec system_spec;
	SDL_AudioSpec sample_spec;
	u32 loaded_sounds;
} SoundManager;

void play_sound_clip(SoundManager* sound_man, SoundClip* clip);

void update_active_channels(SoundManager* sound_man);

