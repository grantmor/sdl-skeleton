#pragma once

#include "types.h"
#include <SDL3/SDL_gamepad.h>

#define AXIS_DEADZONE 250

typedef enum {
	NEGATIVE,
	POSITIVE
} Sign;

bool is_axis_pressed(SDL_Gamepad* gp, SDL_GamepadAxis axis, Sign sign, i16 threshold)
{
	//i8 pn = ((sign == POSITIVE) ? 1 : -1);
	if (sign)
	{
		return (SDL_GetGamepadAxis(gp, axis) > threshold);	
	}
	else
	{
		return (SDL_GetGamepadAxis(gp, axis) < threshold);	
	}
}

void platform_input(AppState* as)
{
	ControllerState* cs = as->controller_state;
	SDL_Gamepad* gp = as->sdl_gamepad;

	// cs->dpad_up = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_UP);
	cs->dpad_up = SDL_GetGamepadButton(as->sdl_gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
	cs->dpad_down =  SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
	cs->dpad_left =  SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
	cs->dpad_right = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

	cs->button_north = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_NORTH);
	cs->button_south = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_SOUTH);
	cs->button_east = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_EAST);
	cs->button_west = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_WEST);

	cs->shoulder_left = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
	cs->shoulder_right = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

	cs->trigger_left = is_axis_pressed(gp, SDL_GAMEPAD_AXIS_LEFT_TRIGGER, POSITIVE, AXIS_DEADZONE);
	cs->trigger_right = is_axis_pressed(gp, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, POSITIVE, AXIS_DEADZONE);

	cs->button_start = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_START);
	cs->button_select = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_BACK);

	cs->click_left = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_LEFT_STICK);
	cs->click_right = SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_RIGHT_STICK);

	cs->axis_lx = SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_LEFTX);
	cs->axis_ly = SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_LEFTY);
	cs->axis_rx = SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_RIGHTX);
	cs->axis_ry = SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_RIGHTY);
}

void game_input(AppState* as)
{
	ControllerState* cs = as->controller_state;	

	// Face Buttons
	if (cs->button_north)
	{
		SDL_Log("North Button");
	}
	if (cs->button_south)
	{
		SDL_Log("South Button");
	}
	if (cs->button_east)
	{
		SDL_Log("East Button");
	}
	if (cs->button_west)
	{
		SDL_Log("West Button");
	}

	// DPAD
	if (cs->dpad_up)
	{
		SDL_Log("DPAD Up");
	}
	if (cs->dpad_down)
	{
		SDL_Log("DPAD Down");
	}
	if (cs->dpad_left)
	{
		SDL_Log("DPAD Left");
	}
	if (cs->dpad_right)
	{
		SDL_Log("DPAD Right");
	}

	// Shoulder
	if (cs->shoulder_left)
	{
		SDL_Log("Left Shoulder");
	}
	if (cs->shoulder_right)
	{
		SDL_Log("Right Shoulder");
	}
	if (cs->trigger_left)
	{
		SDL_Log("Left Trigger");
	}
	if (cs->trigger_right)
	{
		SDL_Log("Right Trigger");
	}

	// Sticks
	if (cs->click_left)
	{
		SDL_Log("Left Stick Click");
	}
	if (cs->click_right)
	{
		SDL_Log("Right Stick Click");
	}

	// System
	if (cs->button_start)
	{
		SDL_Log("Start Button");
	}
	if (cs->button_select)
	{
		SDL_Log("Select Button");
	}

	// Analog Axes
	SDL_Log("Axis Left X Value: %i", cs->axis_lx);
	SDL_Log("Axis Left Y Value: %i", cs->axis_ly);
	SDL_Log("Axis Right X Value: %i", cs->axis_rx);
	SDL_Log("Axis Right Y Value: %i", cs->axis_ry);
}
