#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "audio.c"
#include "game_update.c"
#include "input.h"

void platform_render(AppState* as)
{
	SDL_Renderer* renderer = as->renderer;

	SDL_SetRenderDrawColorFloat(renderer, 0.0,0.0,0.0,1.0);
	SDL_RenderClear(renderer);

	// Just here to visualize screen area and letterboxing
	SDL_SetRenderDrawColor(renderer, 0.0, 0.0, 255, 1.0);
	SDL_FRect rect = (SDL_FRect) {0.0,0.0,320.0,180.0};
	SDL_RenderFillRect(renderer, &rect);
	
	SDL_RenderPresent(as->renderer);
}

void play_sound_clip(SoundManager* sound_man, SoundClip* clip)
{
	for (u64 c=0; c<NUM_SFX_CHANNELS; c++)	
	{
		u64 queued = SDL_GetAudioStreamQueued(sound_man->stream_pool[c]);

		if (queued == 0)
		{
			if (SDL_PutAudioStreamData(sound_man->stream_pool[c], clip->data, clip->length))
			{
				//SDL_Log("Error! Failed to put audio samples in channel %llu", c);
				SDL_FlushAudioStream(sound_man->stream_pool[c]);
			}
			break;
		}
	}
}

void platform_audio(AppState* as)
{
	// Test Audio
	SoundManager* sound_man = as->sound_manager;
	SoundClip* sound_clip = as->sound_clip;

	//SDL_Log("bytes queued: %i", SDL_GetAudioStreamQueued(stream));
	if (as->game_input->keyboard_state.key[KEY_SPACE] == BUTTON_PRESSED)
	{
		play_sound_clip(sound_man, sound_clip);
	}
}


SDL_AppResult platform_iterate(AppState* as)
{
	game_step(as);
	platform_render(as);
	platform_audio(as);

	return SDL_APP_CONTINUE;
}
