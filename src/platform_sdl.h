#pragma once

#include <SDL3/SDL.h>
#include "types.h"

typedef struct {
	i16 axis_lx;
	i16 axis_ly;
	i16 axis_rx;
	i16 axis_ry; 

	i16 axis_lt;
	i16 axis_rt; 

	u8 button_north;
	u8 button_south;
	u8 button_east;
	u8 button_west;
	
	u8 dpad_up;
	u8 dpad_down;
	u8 dpad_left;
	u8 dpad_right;

	u8 shoulder_left;	
	u8 shoulder_right;

	u8 trigger_left;
	u8 trigger_right;

	u8 click_left;
	u8 click_right;

	u8 button_start;
	u8 button_select;
} ControllerState;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	u64 last_time;

	//Input
	SDL_Gamepad* sdl_gamepad;
	ControllerState* controller_state;
} AppState;
