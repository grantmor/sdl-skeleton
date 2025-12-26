#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <wchar.h>

#include "audio.h"
#include "types.h"
#include "platform_sdl.h"

static const char RES_DIR[] = "res";
static const char SFX_DIR[] = "sound";

SDL_EnumerationResult load_wavs(void* userdata, const char* dir, const char* file)
{
	SoundManager* sound_manager = (SoundManager*) userdata;

		SoundClip sound = {
			.path = NULL,
			.data = NULL,
			.length = 0
		};

		SDL_asprintf(&sound.path, "%s%s", dir, file);

		if (!SDL_LoadWAV(sound.path, &sound_manager->sample_spec, &sound.data, &sound.length))
		{
			SDL_Log("Couldn't load wav file: %s", SDL_GetError());
		}
		// TODO: Log successful load here

		//SDL_free(sound.path);
		sound_manager->clips[sound_manager->loaded_sounds] = sound;	
		sound_manager->loaded_sounds++;

		return SDL_ENUM_CONTINUE;
}

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


	// Input
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

	// Textures
	as->sprite_atlas.path = "res/image/sprites.bmp";
	platform_sprite_atlas_load(as->renderer, &as->sprite_atlas);

	// Audio
	SoundManager* sound_manager = &as->sound_manager;
	as->sound_manager.loaded_sounds = 0;
	// as->sound_manager = sound_manager;

	// Load WAV
	as->sound_manager.system_spec = (SDL_AudioSpec)
	{
		.format = SDL_AUDIO_S16LE,
		.channels = 2,
		.freq = 48000,
	};

	as->sound_manager.sample_spec = (SDL_AudioSpec)
	{
		.format = SDL_AUDIO_S16LE,
		.channels = 1,
		.freq = 44100,
	};

	// TODO: get everything in dir and set path constants
	// for (usize s=0; s<1; s++) //TODO: number of sounds in dir
	// {
		

	// }
	// Load wavs
	as->sound_manager.loaded_sounds = 0;
	char* sound_dir;
	SDL_asprintf(&sound_dir, "%s%s/%s", SDL_GetBasePath(), RES_DIR, SFX_DIR);
	SDL_EnumerateDirectory(sound_dir, load_wavs, &as->sound_manager);

	// Initialize Audio
	SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &as->sound_manager.system_spec);
	if (!device)
	{
		SDL_Log("Failed to initialize audio device: %s", SDL_GetError());
	}
	as->sound_manager.device_id = device;

	for (usize s=0; s<NUM_SFX_CHANNELS; s++)
	{
		SDL_AudioStream* stream = SDL_CreateAudioStream(&as->sound_manager.sample_spec, &as->sound_manager.system_spec);
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
