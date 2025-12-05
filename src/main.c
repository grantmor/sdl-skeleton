#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "types.h"

#include "iterate.c"
#include "init.c"
#include "event.c"
#include "quit.c"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[])
{
	return platform_init(appstate);
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return platform_event(appstate, event);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	return platform_iterate(appstate);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	platform_quit(appstate);
}
