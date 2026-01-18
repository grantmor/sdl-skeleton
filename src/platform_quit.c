#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "types.h"

// ifdef
#include "render_sdl_2d.h"

void platform_quit(AppState* as)
{
	render_free();
	// SDL_DestroyRenderer(as->renderer);
	SDL_DestroyWindow(as->window);

	SDL_free(as);
}
