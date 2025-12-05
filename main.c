#include <SDL3/SDL_properties.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[])
{
	SDL_SetAppMetadata("SDL3 Skeleton", "0.1", "sgz");

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize SDL: %s.", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("SDL3 Skeleton", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer))
	{
		SDL_Log("Failed to create window/renderer: %s.", SDL_GetError());
		return SDL_APP_FAILURE;		
	}

	SDL_SetRenderLogicalPresentation(renderer, 320, 180, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
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
				SDL_WindowFlags flags = SDL_GetWindowFlags(window);
				if (flags & SDL_WINDOW_FULLSCREEN)
				{
					SDL_SetWindowFullscreen(window, false);
				} else
				{
					SDL_SetWindowFullscreen(window, true);
				}
				break;
			}
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_SetRenderDrawColorFloat(renderer, 0.0,0.0,1.0,1.0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
