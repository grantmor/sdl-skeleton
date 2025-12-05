#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "types.h"

#include "platform_iterate.c"
#include "platform_init.c"
#include "platform_event.c"
#include "platform_quit.c"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[])
{
	return platform_init(appstate);
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return platform_event( (AppState*) appstate, event);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	return platform_iterate( (AppState*) appstate);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	platform_quit( (AppState*) appstate);
}
