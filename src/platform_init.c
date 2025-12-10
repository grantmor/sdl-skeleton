#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include "types.h"
#include "platform_sdl.h"

SDL_AppResult platform_init(void** appstate)
{
	SDL_SetAppMetadata("SDL3 Skeleton", "0.1", "sgz");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

	/*
	if (init_result)
	{
		SDL_Log("Failed to initialize SDL: %s.", SDL_GetError());
		
		return SDL_APP_FAILURE;
	}
	*/

	AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));
	//TODO: error handling

	// Video
	if (!SDL_CreateWindowAndRenderer("SDL3 Skeleton", 640, 480, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer))
	{
		SDL_Log("Failed to create window/renderer: %s.", SDL_GetError());
		return SDL_APP_FAILURE;		
	}

	SDL_SetRenderLogicalPresentation(as->renderer, 320, 180, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Input
	i32 num_gamepads; 
	SDL_JoystickID *ids = SDL_GetGamepads(&num_gamepads);

	// Handle multiple controllers later
	if (num_gamepads > 0)
	{
		SDL_Gamepad* controller = SDL_OpenGamepad(ids[0]);
		if (!controller)
		{
			SDL_Log("Failed to open gamepad: %s\n", SDL_GetError());
		}
		else
		{
			SDL_Log("Gamepad Name: %s\n", SDL_GetGamepadName(controller));
			as->sdl_gamepad = controller;
			ControllerState* cs = SDL_calloc(1, sizeof(ControllerState));
			as->controller_state = cs;
		}

	}

	KeyboardState* ks = (KeyboardState*) SDL_calloc(1, sizeof(KeyboardState));
	as->keyboard_state = ks;

	// Time
	Time* time = (Time*) SDL_calloc(1, sizeof(Time));
	time->last_time = (f64)SDL_GetPerformanceCounter() / (f64) SDL_GetPerformanceFrequency();
	time->frame_counter = 0;
	time->cur_time = 0;
	time->dt = 0;
	time->fps_avg = 0;

	as->time = time;
	
	*appstate = as;
	return SDL_APP_CONTINUE;
}
