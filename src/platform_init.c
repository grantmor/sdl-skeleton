#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <wchar.h>

#include "types.h"
#include "platform_sdl.h"

#include "audio.c"

SDL_AppResult platform_init(void** appstate)
{
	SDL_SetAppMetadata("SDL3 Skeleton", "0.1", "sgz");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO);

	/*
	if (init_result)
	{
		SDL_Log("Failed to initialize SDL: %s.", SDL_GetError());
		
		return SDL_APP_FAILURE;
	}
	*/

	AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));
	//TODO: error handling

	// Video
	if (!SDL_CreateWindowAndRenderer("SDL3 Skeleton", 640, 480, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer))
	{
		SDL_Log("Failed to create window/renderer: %s.", SDL_GetError());
		return SDL_APP_FAILURE;		
	}

	SDL_SetRenderLogicalPresentation(as->renderer, 320, 180, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Pump events to ensure gamepad can be accessed
	SDL_PumpEvents();
	SDL_UpdateGamepads();
	i32 num_gamepads; 
	SDL_JoystickID *ids = SDL_GetGamepads(&num_gamepads);
	
	// Handle multiple controllers later
	if (num_gamepads > 0)
	{
		SDL_Gamepad* controller = SDL_OpenGamepad(ids[0]);
		if (!controller)
		{
			SDL_Log("Failed to open gamepad: %s\n", SDL_GetError());
		}
		else
		{
			SDL_Log("Gamepad Name: %s\n", SDL_GetGamepadName(controller));
			as->platform_input.platform_gamepad = controller;
		}
	}

	// Time
	Time time = {
		.last_time = (f64)SDL_GetPerformanceCounter() / (f64) SDL_GetPerformanceFrequency(),
		.frame_counter = 0,
		.cur_time = 0,
		.dt = 0,
		.fps_avg = 0
	};
	as->time = time;

	// Audio
	SoundManager sound_manager = {0};
	as->sound_manager = sound_manager;

	// Load WAV
	SoundClip* sound = (SoundClip*) SDL_calloc(1, sizeof(SoundClip));
	
	sound->path = "res/sound";
	sound->data = NULL;
	sound->length = 0;

	as->sound_clip = sound;

	SDL_AudioSpec system_spec;
	system_spec.format = SDL_AUDIO_S16LE;
	system_spec.channels = 2;
	system_spec.freq = 48000;

	SDL_AudioSpec sample_spec;
	sample_spec.format = SDL_AUDIO_S16LE;
	sample_spec.channels = 1;
	sample_spec.freq = 44100;

	SDL_asprintf(&sound->path, "%sres/sound/coin.wav", SDL_GetBasePath());

	if (!SDL_LoadWAV(sound->path, &sample_spec, &sound->data, &sound->length))
	{
		SDL_Log("Couldn't load wav file: %s", SDL_GetError());
	}

	SDL_free(sound->path);

	// Initialize Audio
	SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &system_spec);

	for (usize s=0; s<NUM_SFX_CHANNELS; s++)
	{
		SDL_AudioStream* stream = SDL_CreateAudioStream(&sample_spec, &system_spec);
		SDL_BindAudioStream(device, stream);

		as->sound_manager.stream_pool[s] = stream;

		if (!stream)
		{
			SDL_Log("Could not open audio device: %s", SDL_GetError());
			//TODO: Crash here?
		}
	}
	SDL_ResumeAudioDevice(device);

	*appstate = as;

	return SDL_APP_CONTINUE;
}
