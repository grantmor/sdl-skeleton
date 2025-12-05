#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"

SDL_AppResult platform_init(void** appstate)
{
	SDL_SetAppMetadata("SDL3 Skeleton", "0.1", "sgz");

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize SDL: %s.", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));
	//TODO: error handling

	if (!SDL_CreateWindowAndRenderer("SDL3 Skeleton", 640, 480, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer))
	{
		SDL_Log("Failed to create window/renderer: %s.", SDL_GetError());
		return SDL_APP_FAILURE;		
	}

	SDL_SetRenderLogicalPresentation(as->renderer, 320, 180, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	*appstate = as;
	
	return SDL_APP_CONTINUE;
}
