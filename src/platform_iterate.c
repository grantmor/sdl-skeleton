// #pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <dlfcn.h>

#include "audio.h"
#include "game_update.h"
#include "platform_sdl.h"

// #ifdef RENDER_SDL_2D
	#include "render_sdl_2d.h"
// #endif

#ifdef __EMSCRIPTEN__
	#include "game_update.c"  // Make sure this file is compiled into the executable
#endif

typedef void (*game_init_fn)(PlatformAPI* platform);
typedef void (*game_step_fn)(AppState* as);
static game_step_fn game_step_ptr = NULL;
static void* game_lib = NULL;
static u64 last_game_reload = 0;

bool should_reload_game()
{
	u64 current_timestamp = platform_file_timestamp_get("./build/native/game_update.so");

	if (current_timestamp == -1)
	{
		return false;
	}

	if (current_timestamp > last_game_reload)
	{
		last_game_reload = current_timestamp;
		return true;
	}
	return false;
}

static bool platform_file_copy(const char *src, const char *dst)
{
    SDL_IOStream *in  = SDL_IOFromFile(src, "rb");
    if (!in) return false;

    SDL_IOStream *out = SDL_IOFromFile(dst, "wb");
    if (!out)
    {
        SDL_CloseIO(in);
        return false;
    }

    char buffer[4096];
    size_t bytes;

    while ((bytes = SDL_ReadIO(in, buffer, sizeof(buffer))) > 0)
    {
        SDL_WriteIO(out, buffer, bytes);
    }

    SDL_CloseIO(in);
    SDL_CloseIO(out);

    return true;
}

void platform_reload_game(void)
{
    const char* compiled_lib = "./build/native/game_update.so";

    // Unload old library first
    if (game_lib)
    {
        dlclose(game_lib);
        game_lib = NULL;
    }

    game_step_ptr = NULL;

    // Copy to a unique filename
    char copied_lib[64];
    SDL_snprintf(copied_lib, sizeof(copied_lib), "./build/native/game_update_load_%llu.so", (unsigned long long) SDL_GetTicks());

    INFO("Copying %s -> %s", compiled_lib, copied_lib);

    if (!platform_file_copy(compiled_lib, copied_lib))
    {
        WARN("Failed to copy game library: %s", SDL_GetError());
        return;
    }

    // Load library
    game_lib = dlopen(copied_lib, RTLD_NOW);
    if (!game_lib)
    {
        const char* err = dlerror();
        WARN("dlopen failed: %s", err ? err : "unknown");
        return;
    }

    // Resolve symbol safely
    dlerror(); // clear old errors
    game_step_ptr = (game_step_fn)dlsym(game_lib, "game_step");
    const char* err = dlerror();
    if (err)
    {
        WARN("dlsym failed: %s", err);
        dlclose(game_lib);
        game_lib = NULL;
        game_step_ptr = NULL;
        return;
    }

    // Testing global pointer table
	// **Resolve and call game_init**
    dlerror();
    game_init_fn init_fn = (game_init_fn)dlsym(game_lib, "game_init");
    err = dlerror();
    if (err)
    {
        WARN("dlsym game_init failed: %s", err);
        dlclose(game_lib);
        game_lib = NULL;
        game_step_ptr = NULL;
        return;
    }

    // Pass the platform API pointer into the game library
    // init_fn(&g_platform_api);
    init_fn(g_platform_api);
    
    INFO("Successfully reloaded game_update.so");
}


void sound_clip_play(SoundManager* sound_man, SoundClip* clip)
{
	for (u64 c=0; c<NUM_SFX_CHANNELS; c++)	
	{
		u64 queued = SDL_GetAudioStreamQueued(sound_man->stream_pool[c]);

		if (queued == 0)
		{
			// if (SDL_PutAudioStreamData(sound_man->stream_pool[c], clip->data, clip->length))
			// {
				//SDL_Log("Error! Failed to put audio samples in channel %llu", c);
			SDL_PutAudioStreamData(sound_man->stream_pool[c], clip->data, clip->length);
			SDL_FlushAudioStream(sound_man->stream_pool[c]);
			// }
			break;
		}
	}
}

void platform_audio(SoundManager* sound_man)
{
	for (u32 s=0; s<sound_man->num_sounds_to_play; s++)
	{
		if (sound_man->playing_sounds[s] != SFX_NO_SOUND)
		{
			sound_clip_play(sound_man, &sound_man->clips[sound_man->playing_sounds[s]]);
		}
	}
}

SDL_AppResult platform_iterate(AppState* as)
{
	#ifndef __EMSCRIPTEN
	if (should_reload_game())
	{
		platform_reload_game();
	}

	if (game_step_ptr)
	{
		game_step_ptr(as);
	}
	#endif

	#ifdef __EMSCRIPTEN__
	game_step(as);
	#endif

	// platform_render(as);
 	render_frame();
	platform_audio(&as->sound_manager);

	return SDL_APP_CONTINUE;
}

