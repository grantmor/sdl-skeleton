#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"

SDL_AppResult game_event(void* appstate, SDL_Event* event)
{
	AppState* as = (AppState*) appstate;

	if (event->type == SDL_EVENT_QUIT)
	{
		return SDL_APP_SUCCESS;
	}

	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		switch (event->key.key)
		{
			case SDLK_ESCAPE:
				return SDL_APP_SUCCESS;
			case SDLK_F:
			{
				SDL_WindowFlags flags = SDL_GetWindowFlags(as->window);
				if (flags & SDL_WINDOW_FULLSCREEN)
				{
					SDL_SetWindowFullscreen(as->window, false);
				} else
				{
					SDL_SetWindowFullscreen(as->window, true);
				}
				break;
			}
		}
	}

	return SDL_APP_CONTINUE;
}
