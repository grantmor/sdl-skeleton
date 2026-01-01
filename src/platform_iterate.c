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
#include "super_lib.c"

#ifdef __EMSCRIPTEN__
#include "game_update.c"  // Make sure this file is compiled into the TU
#endif

typedef void (*game_step_fn)(AppState* as);
static game_step_fn game_step_ptr = NULL;
static void* game_lib = NULL;
static u64 last_game_reload = 0;


bool should_reload_game()
{
	u64 current_timestamp = file_get_timestamp("./build/native/game_update.so");

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

static bool copy_game_library(const char *src, const char *dst)
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

    SDL_Log("Copying %s -> %s", compiled_lib, copied_lib);

    if (!copy_game_library(compiled_lib, copied_lib))
    {
        SDL_Log("Failed to copy game library: %s", SDL_GetError());
        return;
    }

    // Load library
    game_lib = dlopen(copied_lib, RTLD_NOW);
    if (!game_lib)
    {
        const char* err = dlerror();
        SDL_Log("dlopen failed: %s", err ? err : "unknown");
        return;
    }

    // Resolve symbol safely
    dlerror(); // clear old errors
    game_step_ptr = (game_step_fn)dlsym(game_lib, "game_step");
    const char* err = dlerror();
    if (err)
    {
        SDL_Log("dlsym failed: %s", err);
        dlclose(game_lib);
        game_lib = NULL;
        game_step_ptr = NULL;
        return;
    }

    SDL_Log("Successfully reloaded game_update.so");
}



void platform_render(AppState* as)
{
	SDL_Renderer* renderer = as->renderer;
	SpriteAtlas* sprite_atlas = &as->sprite_atlas;

	if (file_get_timestamp(sprite_atlas->path) > sprite_atlas->modified)
	{
		platform_sprite_atlas_load(renderer, sprite_atlas);
	}

	SDL_SetRenderDrawColorFloat(renderer, 0.0,0.0,0.0,1.0);
	SDL_RenderClear(renderer);

	// Layer 0 - Just here to visualize screen area and letterboxing
	SDL_SetRenderDrawColor(renderer, 0.0, 0.0, 255, 255);
	SDL_FRect rect = (SDL_FRect) {0.0,0.0,320.0,180.0};
	SDL_RenderFillRect(renderer, &rect);

	// Layer 1 - Testing Sprite Atlas
	SDL_FRect sprite_rect = (SDL_FRect) {0.0,0.0,0.0,0.0};
	SDL_GetTextureSize(sprite_atlas->atlas, &sprite_rect.w, &sprite_rect.h);
	SDL_RenderTexture(renderer, sprite_atlas->atlas, NULL, &sprite_rect);

	SDL_RenderPresent(renderer);
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
	// SDL_Log("platform_audio() entered");
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

	platform_render(as);
	platform_audio(&as->sound_manager);

	return SDL_APP_CONTINUE;
}

