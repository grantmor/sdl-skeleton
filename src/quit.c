#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "types.h"

void  game_quit(void* appstate)
{
	AppState* as = (AppState*) appstate;

	SDL_DestroyRenderer(as->renderer);
	SDL_DestroyWindow(as->window);


	SDL_free(as);
}
