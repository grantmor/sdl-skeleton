#pragma once

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

#include "types.h"
#include "platform_sdl.h"

#include "input.h"


//TODO: Add features of mapping keys and gamepad controls to "Actions"


bool is_axis_pressed(SDL_Gamepad* gp, SDL_GamepadAxis axis, Sign sign, i16 threshold)
{
	if (sign)
	{
		return (SDL_GetGamepadAxis(gp, axis) > threshold);	
	}
	else
	{
		return (SDL_GetGamepadAxis(gp, axis) < threshold);	
	}
}

void platform_gamepad_button_state
(
	SDL_Gamepad* gp,
	SDL_GamepadButton button,
	PlatformControllerState* state_prev,
	ControllerState* cs
)
{
	b8 prev_button_state = state_prev->button[button];
	b8 button_state = SDL_GetGamepadButton(gp, button);

	if (button_state && prev_button_state)
	{
		cs->button[button] = BUTTON_HELD;
	}
	else if (button_state && !prev_button_state)
	{
		cs->button[button] = BUTTON_PRESSED;	
	}
	else if (!button_state && prev_button_state)
	{
		cs->button[button] = BUTTON_RELEASED;	
	}
	else if (!button_state && !prev_button_state)
	{
		cs->button[button] = BUTTON_IDLE;	
	}

	state_prev->button[button] = button_state;
}

void platform_gamepad_button_state_analog
(
	SDL_Gamepad* gp,
	ControllerMap button,
	Sign sign,
	PlatformControllerState* state_prev,
	ControllerState* cs
)
{

	SDL_GamepadAxis axis;

	switch (button)
	{
		case TRIGGER_LEFT:		
			axis = SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
			break;
		case TRIGGER_RIGHT:		
			axis = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
			break;
		default:
			SDL_Log("ERROR: Digital button passed to platform_gamepad_button_state_analog!");
	}

	b8 prev_button_state = state_prev->button[button];
	b8 button_state = is_axis_pressed(gp, axis, sign, AXIS_DEADZONE);

	if (button_state && prev_button_state)
	{
		cs->button[button] = BUTTON_HELD;
	}
	else if (button_state && !prev_button_state)
	{
		cs->button[button] = BUTTON_PRESSED;	
	}
	else if (!button_state && prev_button_state)
	{
		cs->button[button] = BUTTON_RELEASED;	
	}
	else if (!button_state && !prev_button_state)
	{
		cs->button[button] = BUTTON_IDLE;	
	}

	state_prev->button[button] = button_state;
}

void platform_key_state
(
	PlatformKeyboardState* state_prev,
	KeyboardState* ks
)
{
	const b8* prev_key_state = state_prev->key;
	const b8* sdl_key_state = SDL_GetKeyboardState(NULL);

	for (u64 c=0; c<NUM_SCANCODES; c++)
	{
		if (sdl_key_state[c] && prev_key_state[c])
		{
			ks->key[c] = BUTTON_HELD;
		}
		else if (sdl_key_state[c] && !prev_key_state[c])
		{
			ks->key[c] = BUTTON_PRESSED;	
		}
		else if (!sdl_key_state[c] && prev_key_state[c])
		{
			ks->key[c] = BUTTON_RELEASED;	
		}
		else if (!sdl_key_state[c] && !prev_key_state[c])
		{
			ks->key[c] = BUTTON_IDLE;	
		}

		state_prev->key[c] = sdl_key_state[c];
	}
}

void platform_mouse_state(PlatformMouseState* pms, MouseState* ms)
{
    SDL_MouseButtonFlags mouse_flags = SDL_GetMouseState(&ms->pos_x, &ms->pos_y);
    SDL_MouseButtonFlags relative_mouse_flags = SDL_GetRelativeMouseState(&ms->rel_x, &ms->rel_y);

    for (u64 b=0; b<NUM_MOUSE_BUTTONS; b++)
    {
		b8 pressed = (mouse_flags & (1 << b)) != 0;

        if (pressed && pms->button[b])
            ms->button[b] = BUTTON_HELD;
        else if (pressed && !pms->button[b])
            ms->button[b] = BUTTON_PRESSED;
        else if (!pressed && pms->button[b])
            ms->button[b] = BUTTON_RELEASED;
        else
            ms->button[b] = BUTTON_IDLE;

        pms->button[b] = pressed;
    }

    pms->pos_x = ms->pos_x;
    pms->pos_y = ms->pos_y;
    pms->rel_x = ms->rel_x;
    pms->rel_y = ms->rel_y;
}

void platform_input(AppState* as)
{

	// FIXME: Segfaults when gamepad not connected

	// Gamepad
	SDL_Gamepad* gp = as->platform_input->platform_gamepad;
	PlatformControllerState* pcs = &as->platform_input->gamepad_state_prev;
	ControllerState* cs = &as->game_input->controller_state;

	// Set current state
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_DPAD_UP, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_DPAD_DOWN, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_DPAD_LEFT, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, pcs, cs);

	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_NORTH, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_SOUTH, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_EAST, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_WEST, pcs, cs);

	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_START, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_BACK, pcs, cs);

	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, pcs, cs);

	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_LEFT_STICK, pcs, cs);
	platform_gamepad_button_state(gp, SDL_GAMEPAD_BUTTON_RIGHT_STICK, pcs, cs);

	platform_gamepad_button_state_analog(gp, TRIGGER_LEFT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, TRIGGER_RIGHT, POSITIVE, pcs, cs);

/* TODO: Sticks as buttons
	platform_gamepad_button_state_analog(gp, SHOULDER_LEFT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_RIGHT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_LEFT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_RIGHT, POSITIVE, pcs, cs);
*/


	// Mouse and Keyboard
	platform_key_state(&as->platform_input->keyboard_prev, &as->game_input->keyboard_state);
	platform_mouse_state(&as->platform_input->mouse_state_prev, &as->game_input->mouse_state);
}

void game_input(AppState* as)
{
	ControllerState* cs = &as->game_input->controller_state;

	// Face Buttons
	if (cs->button[BUTTON_NORTH] == BUTTON_HELD) SDL_Log("North Button held");
	if (cs->button[BUTTON_SOUTH] == BUTTON_HELD) SDL_Log("South Button held");
	if (cs->button[BUTTON_EAST]  == BUTTON_HELD) SDL_Log("East Button held");
	if (cs->button[BUTTON_WEST]  == BUTTON_HELD) SDL_Log("West Button held");

	// DPAD
	if (cs->button[DPAD_UP]    == BUTTON_HELD) SDL_Log("DPAD Up held");
	if (cs->button[DPAD_DOWN]  == BUTTON_HELD) SDL_Log("DPAD Down held");
	if (cs->button[DPAD_LEFT]  == BUTTON_HELD) SDL_Log("DPAD Left held");
	if (cs->button[DPAD_RIGHT] == BUTTON_HELD) SDL_Log("DPAD Right held");

	// Shoulder
	if (cs->button[SHOULDER_LEFT]  == BUTTON_HELD) SDL_Log("Left Shoulder held");
	if (cs->button[SHOULDER_RIGHT] == BUTTON_HELD) SDL_Log("Right Shoulder held");

	// Triggers
	if (cs->button[TRIGGER_LEFT]  == BUTTON_HELD) SDL_Log("Left Trigger held");
	if (cs->button[TRIGGER_RIGHT] == BUTTON_HELD) SDL_Log("Right Trigger held");

	// Sticks
	if (cs->button[CLICK_LEFT]  == BUTTON_HELD) SDL_Log("Left Stick Click held");
	if (cs->button[CLICK_RIGHT] == BUTTON_HELD) SDL_Log("Right Stick Click held");

	// System
	if (cs->button[BUTTON_START] == BUTTON_HELD) SDL_Log("Start Button held");
	if (cs->button[BUTTON_SELECT]  == BUTTON_HELD) SDL_Log("Select Button held");

	// Keyboard
	KeyboardState* ks = &as->game_input->keyboard_state;

	if (ks->key[KEY_0] == BUTTON_HELD) SDL_Log("0 key held");
	if (ks->key[KEY_1] == BUTTON_HELD) SDL_Log("1 key held");
	if (ks->key[KEY_2] == BUTTON_HELD) SDL_Log("2 key held");
	if (ks->key[KEY_3] == BUTTON_HELD) SDL_Log("3 key held");
	if (ks->key[KEY_4] == BUTTON_HELD) SDL_Log("4 key held");
	if (ks->key[KEY_5] == BUTTON_HELD) SDL_Log("5 key held");
	if (ks->key[KEY_6] == BUTTON_HELD) SDL_Log("6 key held");
	if (ks->key[KEY_7] == BUTTON_HELD) SDL_Log("7 key held");
	if (ks->key[KEY_8] == BUTTON_HELD) SDL_Log("8 key held");
	if (ks->key[KEY_9] == BUTTON_HELD) SDL_Log("9 key held");

	if (ks->key[KEY_A] == BUTTON_HELD) SDL_Log("A key held");
	if (ks->key[KEY_B] == BUTTON_HELD) SDL_Log("B key held");
	if (ks->key[KEY_C] == BUTTON_HELD) SDL_Log("C key held");
	if (ks->key[KEY_D] == BUTTON_HELD) SDL_Log("D key held");
	if (ks->key[KEY_E] == BUTTON_HELD) SDL_Log("E key held");
	if (ks->key[KEY_F] == BUTTON_HELD) SDL_Log("F key held");
	if (ks->key[KEY_G] == BUTTON_HELD) SDL_Log("G key held");
	if (ks->key[KEY_H] == BUTTON_HELD) SDL_Log("H key held");
	if (ks->key[KEY_I] == BUTTON_HELD) SDL_Log("I key held");
	if (ks->key[KEY_J] == BUTTON_HELD) SDL_Log("J key held");
	if (ks->key[KEY_K] == BUTTON_HELD) SDL_Log("K key held");
	if (ks->key[KEY_L] == BUTTON_HELD) SDL_Log("L key held");
	if (ks->key[KEY_M] == BUTTON_HELD) SDL_Log("M key held");
	if (ks->key[KEY_N] == BUTTON_HELD) SDL_Log("N key held");
	if (ks->key[KEY_O] == BUTTON_HELD) SDL_Log("O key held");
	if (ks->key[KEY_P] == BUTTON_HELD) SDL_Log("P key held");
	if (ks->key[KEY_Q] == BUTTON_HELD) SDL_Log("Q key held");
	if (ks->key[KEY_R] == BUTTON_HELD) SDL_Log("R key held");
	if (ks->key[KEY_S] == BUTTON_HELD) SDL_Log("S key held");
	if (ks->key[KEY_T] == BUTTON_HELD) SDL_Log("T key held");
	if (ks->key[KEY_U] == BUTTON_HELD) SDL_Log("U key held");
	if (ks->key[KEY_V] == BUTTON_HELD) SDL_Log("V key held");
	if (ks->key[KEY_W] == BUTTON_HELD) SDL_Log("W key held");
	if (ks->key[KEY_X] == BUTTON_HELD) SDL_Log("X key held");
	if (ks->key[KEY_Y] == BUTTON_HELD) SDL_Log("Y key held");
	if (ks->key[KEY_Z] == BUTTON_HELD) SDL_Log("Z key held");

	if (ks->key[KEY_MINUS] == BUTTON_HELD) SDL_Log("MINUS key held");
	if (ks->key[KEY_EQUALS] == BUTTON_HELD) SDL_Log("EQUALS key held");
	if (ks->key[KEY_LEFTBRACKET] == BUTTON_HELD) SDL_Log("LEFTBRACKET key held");
	if (ks->key[KEY_RIGHTBRACKET] == BUTTON_HELD) SDL_Log("RIGHTBRACKET key held");
	if (ks->key[KEY_BACKSLASH] == BUTTON_HELD) SDL_Log("BACKSLASH key held");
	if (ks->key[KEY_SEMICOLON] == BUTTON_HELD) SDL_Log("SEMICOLON key held");
	if (ks->key[KEY_APOSTROPHE] == BUTTON_HELD) SDL_Log("APOSTROPHE key held");
	if (ks->key[KEY_GRAVE] == BUTTON_HELD) SDL_Log("GRAVE key held");
	if (ks->key[KEY_COMMA] == BUTTON_HELD) SDL_Log("COMMA key held");
	if (ks->key[KEY_PERIOD] == BUTTON_HELD) SDL_Log("PERIOD key held");
	if (ks->key[KEY_SLASH] == BUTTON_HELD) SDL_Log("SLASH key held");

	if (ks->key[KEY_ESCAPE] == BUTTON_HELD) SDL_Log("ESCAPE key held");
	if (ks->key[KEY_TAB] == BUTTON_HELD) SDL_Log("TAB key held");
	if (ks->key[KEY_SPACE] == BUTTON_HELD) SDL_Log("SPACE key held");

	if (ks->key[KEY_F1] == BUTTON_HELD) SDL_Log("F1 key held");
	if (ks->key[KEY_F2] == BUTTON_HELD) SDL_Log("F2 key held");
	if (ks->key[KEY_F3] == BUTTON_HELD) SDL_Log("F3 key held");
	if (ks->key[KEY_F4] == BUTTON_HELD) SDL_Log("F4 key held");
	if (ks->key[KEY_F5] == BUTTON_HELD) SDL_Log("F5 key held");
	if (ks->key[KEY_F6] == BUTTON_HELD) SDL_Log("F6 key held");
	if (ks->key[KEY_F7] == BUTTON_HELD) SDL_Log("F7 key held");
	if (ks->key[KEY_F8] == BUTTON_HELD) SDL_Log("F8 key held");
	if (ks->key[KEY_F9] == BUTTON_HELD) SDL_Log("F9 key held");
	if (ks->key[KEY_F10] == BUTTON_HELD) SDL_Log("F10 key held");
	if (ks->key[KEY_F11] == BUTTON_HELD) SDL_Log("F11 key held");
	if (ks->key[KEY_F12] == BUTTON_HELD) SDL_Log("F12 key held");

	// Mouse
	PlatformMouseState* pms = &as->platform_input->mouse_state_prev;
	MouseState* ms = &as->game_input->mouse_state;

	SDL_Log("Axis Right X Value: %f", ms->pos_x);
	SDL_Log("Axis Right Y Value: %f", ms->pos_y);

	SDL_Log("Axis Right X Relative Value: %f", ms->rel_x);
	SDL_Log("Axis Right Y Relative Value: %f", ms->rel_y);

	for (u64 b=0; b<NUM_MOUSE_BUTTONS; b++)
	{
		if (ms->button[b] == BUTTON_HELD)
		{
			if (b == MOUSE_BUTTON_LEFT) SDL_Log("Left Mouse Button Held");
			if (b == MOUSE_BUTTON_RIGHT) SDL_Log("Right Mouse Button Held");
			if (b == MOUSE_BUTTON_MIDDLE) SDL_Log("Middle Mouse Button Held");
			if (b == MOUSE_BUTTON_SIDE_1) SDL_Log("Mouse Side 1 Button Held");
			if (b == MOUSE_BUTTON_SIDE_2) SDL_Log("Mouse Side 2 Button Held");
		}
	}
}
