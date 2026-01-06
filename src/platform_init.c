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

#include <stdarg.h>

#include "audio.h"
#include "super_lib.h"
#include "types.h"
#include "super_lib.c"
#include "platform_sdl.h"


static const char RES_DIR[] = "res";
static const char IMG_DIR[] = "image";
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
	// Test Logging
	platform_trace("trace test");
	platform_info("info test");
	platform_warn("warn test");
	platform_error("error test");

	u64 arena_cap = 256;
	// u8 backing_buffer[arena_cap];
	Arena arena = arena_alloc_make(as->test_arena, arena_cap);
	Arena scratch = arena_alloc_make(as->test_arena, arena_cap);

	MemoryContext memctx = (MemoryContext) {.arena = &arena, .scratch = &scratch};
	SDL_Log("context created");

	/*
	SDL_Log("arena.used: %d", arena.used);
	SDL_Log("arena.capacity: %d", arena.capacity);
	u8* zero_zone = arena_alloc(&arena, 8);
	SDL_Log("arena.used: %d", arena.used);
	SDL_Log("arena.capacity: %d", arena.capacity);
	u8* one_zone = arena_alloc(&arena, 8);

	for (usize b=0; b<8; b++)
	{
		zero_zone[b] = 0;
	}

	for (usize b=arena.used; b<arena.used+8; b++)
	{
		zero_zone[b] = 1;
	}

	for (u64 b=0; b<arena_cap; b++)
	{
		SDL_Log("%d", backing_buffer[b]);
	}
	*/

	// String a = str_make(arena, "test_a");
	// String b = str_make(&arena, "test_b");
	// platform_warn(str_to_cstring(&arena, a));
	// platform_warn(str_to_cstring(&arena, b));

	// String c = str_concat(&arena, a, b);
	// platform_warn(str_to_cstring(&arena, c));

	// Test str
	// platform_info(str_put_2_c(&arena, "testklasdjfkljasdkl;asdj_a ", "test_jklasdjfklasdj")); 

	platform_error(str_to_cstring(&memctx, str_put(&memctx, 3, "stringa", "stringb", "stringc")));
	SDL_Log("platform_error run");

	/*
	for (usize b=0; b<arena_cap; b++)
	{
		SDL_Log("%c", backing_buffer[b]);	
	}
	*/

	//TODO: error handling

	// Video
	if (!SDL_CreateWindowAndRenderer("SDL3 Skeleton", 1280, 720, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer))
	{
		SDL_Log("Failed to create window/renderer: %s.", SDL_GetError());
		return SDL_APP_FAILURE;		
	}

	SDL_SetRenderLogicalPresentation(as->renderer, 640, 360, SDL_LOGICAL_PRESENTATION_LETTERBOX);


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

	for (usize d=0; d<6; d++)
	{
		as->game_input.controller_state.deadzone[d] = 1000;
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

	// Load wavs
	as->sound_manager.loaded_sounds = 0;
	char* sound_dir = "res/sound";
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

	// init audio playing
	for (u32 s=0; s<MAX_SOUNDS; s++)
	{
		as->sound_manager.playing_sounds[s] = SFX_NO_SOUND;
	}
	as->sound_manager.num_sounds_to_play = 0;

	*appstate = as;

	return SDL_APP_CONTINUE;
}
