#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"
#include "game_update.c"

void platform_render(AppState* as)
{
	SDL_SetRenderDrawColorFloat(as->renderer, 0.0,0.0,1.0,1.0);
	SDL_RenderClear(as->renderer);
	SDL_RenderPresent(as->renderer);
}

void platform_audio(AppState* as)
{
	
}

SDL_AppResult platform_iterate(AppState* as)
{
	game_step(as);
	platform_render(as);
	platform_audio(as);

	return SDL_APP_CONTINUE;
}
