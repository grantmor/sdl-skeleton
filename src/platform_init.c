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

#include <SDL3/SDL_video.h>
#include <stdarg.h>

#include "audio.h"
#include "super_lib.h"
#include "types.h"
#include "super_lib.c"
#include "platform_sdl.h"


// #ifdef RENDER_SDL_2D
 	#include "render_sdl_2d.h"
	#include "render_sdl_2d.c"
// #endif

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
			ERROR("Couldn't load wav file: %s", SDL_GetError());
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

	//FIXME: Handle errors
	AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));

	// Function pointers for platform API
	g_platform_api = &as->platform_api;
	as->platform_api = (PlatformAPI)
	{
		.platform_trace_ptr = platform_trace,
		.platform_info_ptr = platform_info,
		.platform_warn_ptr = platform_warn,
		.platform_error_ptr = platform_error,
		.render_frame_ptr = render_frame,
	};

	Arena app_arena = arena_alloc_make(as->memory.app_arena_buffer, ARENA_APP_SIZE);
	Arena app_scratch = arena_alloc_make(as->memory.app_scratch_buffer, SCRATCH_APP_SIZE);

	Arena game_arena = arena_alloc_make(as->game_state.game_arena_buffer, ARENA_GAME_SIZE);
	Arena game_scratch = arena_alloc_make(as->game_state.game_scratch_buffer, SCRATCH_GAME_SIZE);

	Arena frame_arena = arena_alloc_make(as->frame_state.frame_arena_buffer, ARENA_FRAME_SIZE);
	Arena frame_scratch = arena_alloc_make(as->frame_state.frame_scratch_buffer, SCRATCH_FRAME_SIZE);

	as->memory.app_arena = app_arena;
	as->memory.app_scratch = app_scratch;
	as->game_state.game_arena = game_arena;
	as->game_state.game_scratch = game_scratch;
	as->frame_state.frame_arena = frame_arena;
	as->frame_state.frame_scratch = frame_scratch;

	MemoryContext mctx = (MemoryContext) {.arena = &app_arena, .scratch = &app_scratch};

	// Window
	v2u win_size = (v2u) {1280, 720};
	SDL_Window* window = SDL_CreateWindow("SDL3 Skeleton", win_size.x, win_size.y, SDL_WINDOW_RESIZABLE);
	as->window = window;

	v2u fb_size = (v2u) {320, 180};
	render_make(window, fb_size);

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
			ERROR("Failed to open gamepad: %s\n", SDL_GetError());
		}
		else
		{
			INFO("Controller Name: %s detected", SDL_GetGamepadName(controller));
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

	// Video

	// FIXME: Hard-coded for now, sprite system automation comes later
    // Populate Animation Frames 
    as->video.tile_frames = (TileFrame*) arena_alloc(mctx.arena, sizeof(TileFrame) * MAX_ANIMATION_FRAMES);
    as->video.tile_frames[0] = (TileFrame) {
        .x = 2,
        .y = 4,
        .w = 20,
        .h = 28,
        .px = 10,
        .py = 28, 
    };

    // Populate Sprite Animation Table
    as->video.sprite_animation_map[MAIN][IDLE] = MAIN_IDLE;
    as->video.animations[MAIN_IDLE] = (SpriteAnimation)
    {
        .anim_speed = 1.0,
        .start_frame = 0,
        .frame_count = 1,
        .loop = true,
    };

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
		ERROR("Failed to initialize audio device: %s", SDL_GetError());
	}
	as->sound_manager.device_id = device;

	for (usize s=0; s<NUM_SFX_CHANNELS; s++)
	{
		SDL_AudioStream* stream = SDL_CreateAudioStream(&as->sound_manager.sample_spec, &as->sound_manager.system_spec);
		SDL_BindAudioStream(device, stream);

		as->sound_manager.stream_pool[s] = stream;

		if (!stream)
		{
			ERROR("Could not open audio device: %s", SDL_GetError());
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
