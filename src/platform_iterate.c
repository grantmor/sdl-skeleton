#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "audio.h"
#include "game_update.c"
#include "input.h"
#include "platform_sdl.h"
#include "super_lib.c"

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

void play_sound_clip(SoundManager* sound_man, SoundClip* clip)
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

// void platform_audio(AppState* as)
void platform_audio(SoundManager* sound_man)
{
	for (u32 s=0; s<sound_man->num_sounds_to_play; s++)
	{
		if (sound_man->playing_sounds[s] != SFX_NO_SOUND)
		{
			play_sound_clip(sound_man, &sound_man->clips[sound_man->playing_sounds[s]]);
		}
	}
}


SDL_AppResult platform_iterate(AppState* as)
{
	game_step(as);
	platform_render(as);
	platform_audio(&as->sound_manager);

	return SDL_APP_CONTINUE;
}

