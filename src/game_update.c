#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

#include "platform_sdl.h"
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

void update_time(Time* time)
{
	//Time* time = as->time;
    u64 pc = SDL_GetPerformanceCounter();
    u64 freq = SDL_GetPerformanceFrequency();
    time->cur_time = (f64)pc / (f64)freq;

    // Compute frame time
    f64 frametime = time->cur_time - time->last_time;
    if (frametime <= 0.0) frametime = 1.0 / 1000.0; // prevent divide by zero
    time->fps_avg = time->fps_avg * 0.9 + (1.0 / frametime) * 0.1;

	// Don't log every frame
    if (time->frame_counter % 2000 == 0)
        SDL_Log("FPS: %0.0f", time->fps_avg);

    // Update last time for next frame
    time->last_time = time->cur_time;
    time->frame_counter++;
}

void game_step(AppState* as)
{
    update_time(as->time);

    platform_input(as);    
    game_input(as);
    game_update(as);
    build_render_list(as);
    build_audio_list(as);

}

