#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "types.h"

#include "input.c"

void game_update()
{
	
}

void build_render_list()
{
	
}

void build_audio_list()
{
	
}

void game_step(AppState* as)
{
	platform_input();	
	game_input();
	game_update();
	build_render_list();
	build_audio_list();
}
