#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>

#include "audio.c"
#include "game_update.c"

void platform_render(AppState* as)
{
	SDL_SetRenderDrawColorFloat(as->renderer, 0.0,0.0,1.0,1.0);
	SDL_RenderClear(as->renderer);
	SDL_RenderPresent(as->renderer);
}

void play_sound_clip(SoundManager* sound_man, SoundClip* clip)
{
	u64* active_flags = &sound_man->active_flags;

	for (u64 c=0; c<NUM_SFX_CHANNELS; c++)	
	{
		u64 mask = 0;
		mask |= ((u64) 1 << c);
		if ((*active_flags & mask) == 0)
		{
			SDL_PutAudioStreamData(sound_man->stream_pool[c], clip->data, clip->length);
			*active_flags = *active_flags & mask;
			break;
		}
	}
}

void platform_audio(AppState* as)
{

	// Test Audio
	// SDL_AudioStream* stream = as->sdl_audio_stream;
	SoundManager* sound_man = as->sound_manager;
	SoundClip* sound_clip = as->sound_clip;

	//SDL_Log("bytes queued: %i", SDL_GetAudioStreamQueued(stream));
	//if (SDL_GetAudioStreamQueued(stream) < sound_clip->length)
	if (as->keyboard_state->KEY_SPACE)
	{
		//SDL_Log("space");
		//SDL_PutAudioStreamData(stream, sound_clip->data, sound_clip->length);
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
