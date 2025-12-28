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

typedef enum {
	SFX_NO_SOUND = -1,
	SFX_JUMP = 0,
	SFX_COIN = 1
} SoundID;

typedef struct {
	SoundClip clips[MAX_SOUNDS];
	SDL_AudioSpec system_spec;
	SDL_AudioSpec sample_spec;
	SoundID playing_sounds[NUM_SFX_CHANNELS]; // Not handling overflow of stream_pool yet
	SDL_AudioDeviceID device_id;
	SDL_AudioStream* stream_pool[NUM_SFX_CHANNELS];
	u32 loaded_sounds;
	u32 num_sounds_to_play;
} SoundManager;

void play_sound_clip(SoundManager* sound_man, SoundClip* clip);

void update_active_channels(SoundManager* sound_man);

