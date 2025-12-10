#pragma once

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_stdinc.h>

#include "types.h"
#include "platform_sdl.h"

#define AXIS_DEADZONE 25

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
	// Gamepad
	ControllerState* cs = as->controller_state;
	SDL_Gamepad* gp = as->sdl_gamepad;

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

	// Keyboard
	KeyboardState* ks = as->keyboard_state;
	const bool* sdl_state = SDL_GetKeyboardState(NULL);

	ks->KEY_0 = sdl_state[SDL_SCANCODE_0];
	ks->KEY_1 = sdl_state[SDL_SCANCODE_1];
	ks->KEY_2 = sdl_state[SDL_SCANCODE_2];
	ks->KEY_3 = sdl_state[SDL_SCANCODE_3];
	ks->KEY_4 = sdl_state[SDL_SCANCODE_4];
	ks->KEY_5 = sdl_state[SDL_SCANCODE_5];
	ks->KEY_6 = sdl_state[SDL_SCANCODE_6];
	ks->KEY_7 = sdl_state[SDL_SCANCODE_7];
	ks->KEY_8 = sdl_state[SDL_SCANCODE_8];
	ks->KEY_9 = sdl_state[SDL_SCANCODE_9];
	ks->KEY_A = sdl_state[SDL_SCANCODE_A];
	ks->KEY_B = sdl_state[SDL_SCANCODE_B];
	ks->KEY_C = sdl_state[SDL_SCANCODE_C];
	ks->KEY_D = sdl_state[SDL_SCANCODE_D];
	ks->KEY_E = sdl_state[SDL_SCANCODE_E];
	ks->KEY_F = sdl_state[SDL_SCANCODE_F];
	ks->KEY_G = sdl_state[SDL_SCANCODE_G];
	ks->KEY_H = sdl_state[SDL_SCANCODE_H];
	ks->KEY_I = sdl_state[SDL_SCANCODE_I];
	ks->KEY_J = sdl_state[SDL_SCANCODE_J];
	ks->KEY_K = sdl_state[SDL_SCANCODE_K];
	ks->KEY_L = sdl_state[SDL_SCANCODE_L];
	ks->KEY_M = sdl_state[SDL_SCANCODE_M];
	ks->KEY_N = sdl_state[SDL_SCANCODE_N];
	ks->KEY_O = sdl_state[SDL_SCANCODE_O];
	ks->KEY_P = sdl_state[SDL_SCANCODE_P];
	ks->KEY_Q = sdl_state[SDL_SCANCODE_Q];
	ks->KEY_R = sdl_state[SDL_SCANCODE_R];
	ks->KEY_S = sdl_state[SDL_SCANCODE_S];
	ks->KEY_T = sdl_state[SDL_SCANCODE_T];
	ks->KEY_U = sdl_state[SDL_SCANCODE_U];
	ks->KEY_V = sdl_state[SDL_SCANCODE_V];
	ks->KEY_W = sdl_state[SDL_SCANCODE_W];
	ks->KEY_X = sdl_state[SDL_SCANCODE_X];
	ks->KEY_Y = sdl_state[SDL_SCANCODE_Y];
	ks->KEY_Z = sdl_state[SDL_SCANCODE_Z];
	ks->KEY_MINUS = sdl_state[SDL_SCANCODE_MINUS];
	ks->KEY_EQUALS = sdl_state[SDL_SCANCODE_EQUALS];
	ks->KEY_LEFTBRACKET = sdl_state[SDL_SCANCODE_LEFTBRACKET];
	ks->KEY_RIGHTBRACKET = sdl_state[SDL_SCANCODE_RIGHTBRACKET];
	ks->KEY_BACKSLASH = sdl_state[SDL_SCANCODE_BACKSLASH];
	ks->KEY_SEMICOLON = sdl_state[SDL_SCANCODE_SEMICOLON];
	ks->KEY_APOSTROPHE = sdl_state[SDL_SCANCODE_APOSTROPHE];
	ks->KEY_GRAVE = sdl_state[SDL_SCANCODE_GRAVE];
	ks->KEY_COMMA = sdl_state[SDL_SCANCODE_COMMA];
	ks->KEY_PERIOD = sdl_state[SDL_SCANCODE_PERIOD];
	ks->KEY_SLASH = sdl_state[SDL_SCANCODE_SLASH];
	ks->KEY_SPACE = sdl_state[SDL_SCANCODE_SPACE];
	ks->KEY_TAB = sdl_state[SDL_SCANCODE_TAB];
}

void game_input(AppState* as)
{
	ControllerState* cs = as->controller_state;	

	// Face Buttons
	if (cs->button_north) SDL_Log("North Button");
	if (cs->button_south) SDL_Log("South Button");
	if (cs->button_east) SDL_Log("East Button");
	if (cs->button_west) SDL_Log("West Button");

	// DPAD
	if (cs->dpad_up) SDL_Log("DPAD Up");
	if (cs->dpad_down) SDL_Log("DPAD Down");
	if (cs->dpad_left) SDL_Log("DPAD Left");
	if (cs->dpad_right) SDL_Log("DPAD Right");

	// Shoulder
	if (cs->shoulder_left) SDL_Log("Left Shoulder");
	if (cs->shoulder_right) SDL_Log("Right Shoulder");
	if (cs->trigger_left) SDL_Log("Left Trigger");
	if (cs->trigger_right) SDL_Log("Right Trigger");

	// Sticks
	if (cs->click_left) SDL_Log("Left Stick Click");
	if (cs->click_right) SDL_Log("Right Stick Click");

	// System
	if (cs->button_start) SDL_Log("Start Button");
	if (cs->button_select) SDL_Log("Select Button");

	// Analog Axes
	if (SDL_abs(cs->axis_lx) > AXIS_DEADZONE) SDL_Log("Axis Left X Value: %i", cs->axis_lx);
	if (SDL_abs(cs->axis_ly) > AXIS_DEADZONE) SDL_Log("Axis Left Y Value: %i", cs->axis_ly);
	if (SDL_abs(cs->axis_rx) > AXIS_DEADZONE) SDL_Log("Axis Right X Value: %i", cs->axis_rx);
	if (SDL_abs(cs->axis_ry) > AXIS_DEADZONE) SDL_Log("Axis Right Y Value: %i", cs->axis_ry);

	// Keyboard
	KeyboardState* ks = as->keyboard_state;

	if (ks->KEY_0) SDL_Log("0 key pressed");
	if (ks->KEY_1) SDL_Log("1 key pressed");
	if (ks->KEY_2) SDL_Log("2 key pressed");
	if (ks->KEY_3) SDL_Log("3 key pressed");
	if (ks->KEY_4) SDL_Log("4 key pressed");
	if (ks->KEY_5) SDL_Log("5 key pressed");
	if (ks->KEY_6) SDL_Log("6 key pressed");
	if (ks->KEY_7) SDL_Log("7 key pressed");
	if (ks->KEY_8) SDL_Log("8 key pressed");
	if (ks->KEY_9) SDL_Log("9 key pressed");
	if (ks->KEY_A) SDL_Log("A key pressed");
	if (ks->KEY_B) SDL_Log("B key pressed");
	if (ks->KEY_C) SDL_Log("C key pressed");
	if (ks->KEY_D) SDL_Log("D key pressed");
	if (ks->KEY_E) SDL_Log("E key pressed");
	if (ks->KEY_F) SDL_Log("F key pressed");
	if (ks->KEY_G) SDL_Log("G key pressed");
	if (ks->KEY_H) SDL_Log("H key pressed");
	if (ks->KEY_I) SDL_Log("I key pressed");
	if (ks->KEY_J) SDL_Log("J key pressed");
	if (ks->KEY_K) SDL_Log("K key pressed");
	if (ks->KEY_L) SDL_Log("L key pressed");
	if (ks->KEY_M) SDL_Log("M key pressed");
	if (ks->KEY_N) SDL_Log("N key pressed");
	if (ks->KEY_O) SDL_Log("O key pressed");
	if (ks->KEY_P) SDL_Log("P key pressed");
	if (ks->KEY_Q) SDL_Log("Q key pressed");
	if (ks->KEY_R) SDL_Log("R key pressed");
	if (ks->KEY_S) SDL_Log("S key pressed");
	if (ks->KEY_T) SDL_Log("T key pressed");
	if (ks->KEY_U) SDL_Log("U key pressed");
	if (ks->KEY_V) SDL_Log("V key pressed");
	if (ks->KEY_W) SDL_Log("W key pressed");
	if (ks->KEY_X) SDL_Log("X key pressed");
	if (ks->KEY_Y) SDL_Log("Y key pressed");
	if (ks->KEY_Z) SDL_Log("Z key pressed");
	if (ks->KEY_MINUS) SDL_Log("MINUS key pressed");
	if (ks->KEY_EQUALS) SDL_Log("EQUALS key pressed");
	if (ks->KEY_LEFTBRACKET) SDL_Log("LEFTBRACKET key pressed");
	if (ks->KEY_RIGHTBRACKET) SDL_Log("RIGHTBRACKET key pressed");
	if (ks->KEY_BACKSLASH) SDL_Log("BACKSLASH key pressed");
	if (ks->KEY_SEMICOLON) SDL_Log("SEMICOLON key pressed");
	if (ks->KEY_APOSTROPHE) SDL_Log("APOSTROPHE key pressed");
	if (ks->KEY_GRAVE) SDL_Log("GRAVE key pressed");
	if (ks->KEY_COMMA) SDL_Log("COMMA key pressed");
	if (ks->KEY_PERIOD) SDL_Log("PERIOD key pressed");
	if (ks->KEY_SLASH) SDL_Log("SLASH key pressed");
	if (ks->KEY_SPACE) SDL_Log("SPACE key pressed");
	if (ks->KEY_TAB) SDL_Log("TAB key pressed");
}
