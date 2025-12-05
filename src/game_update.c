#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"

#include "input.c"

void game_update(AppState* as)
{
	
}

void build_render_list(AppState* as)
{
	
}

void build_audio_list(AppState* as)
{
	
}

void game_step(AppState* as)
{
	platform_input(as);	
	game_input(as);
	game_update(as);
	build_render_list(as);
	build_audio_list(as);
}
