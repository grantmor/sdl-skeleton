#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"

SDL_AppResult platform_iterate(void* appstate)
{
	AppState* as = (AppState*) appstate;
	SDL_SetRenderDrawColorFloat(as->renderer, 0.0,0.0,1.0,1.0);
	SDL_RenderClear(as->renderer);
	SDL_RenderPresent(as->renderer);

	return SDL_APP_CONTINUE;
}
