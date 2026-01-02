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
	b8 button_state = is_axis_pressed(gp, axis, sign, cs->deadzone[axis]);

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


void platform_gamepad_axis_state
(
	SDL_Gamepad* gp,
	SDL_GamepadAxis axis,
	PlatformControllerState* state_prev,
	ControllerState* cs
)
{
	i32 value = SDL_GetGamepadAxis(gp, axis);
	i8 sign = (value > 0) - (value < 0);
	i32 magnitude = sl_mini(sl_absi(value), SDL_MAX_SINT16);
	i16 clamped = (i16) (magnitude * sign);

	// TODO: Calculate and store relative later?
	state_prev->axis[axis] = cs->axis[axis];
	cs->axis[axis] = clamped; 
}

void platform_input(
	PlatformInput* platform_input,
	GameInput* game_input
)
{
	// FIXME: Segfaults when gamepad not connected

	// Gamepad
	SDL_Gamepad* gp = platform_input->platform_gamepad;
	PlatformControllerState* pcs = &platform_input->gamepad_state_prev;
	ControllerState* cs = &game_input->controller_state;

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

	// Analog Axis Values
	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_LEFTX, pcs, cs);
	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_LEFTY, pcs, cs);
	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_RIGHTX, pcs, cs);
	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_RIGHTY, pcs, cs);

	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_LEFT_TRIGGER, pcs, cs);
	platform_gamepad_axis_state(gp, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, pcs, cs);

// TODO: Sticks as buttons
/*
	platform_gamepad_button_state_analog(gp, SHOULDER_LEFT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_RIGHT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_LEFT, POSITIVE, pcs, cs);
	platform_gamepad_button_state_analog(gp, SHOULDER_RIGHT, POSITIVE, pcs, cs);
*/

	// Mouse and Keyboard
	platform_key_state(&platform_input->keyboard_prev, &game_input->keyboard_state);
	platform_mouse_state(&platform_input->mouse_state_prev, &game_input->mouse_state);
}

// Nice API
 
// Buttons
b8 button_pressed(ControllerState* controller, ControllerMap button)
{
	return (controller->button[button] == BUTTON_PRESSED);
}

b8 button_held(ControllerState* controller, ControllerMap button)
{
	return (controller->button[button] == BUTTON_HELD);
}

b8 button_released(ControllerState* controller, ControllerMap button)
{
	return (controller->button[button] == BUTTON_RELEASED);
}

b8 button_idle(ControllerState* controller, ControllerMap button)
{
	return (controller->button[button] == BUTTON_IDLE);
}

// TODO: DEADZONE
// Sticks and Triggers

// TODO: Consider making this linear from deadzone instead of from absolute zero
// FIXME: Make this branchless?
i16 deadzone(ControllerState* cs, ControllerAxisMap axis)
{
	i16 val = cs->axis[axis];
	i16 deadzone = cs->deadzone[axis];

	if (sl_absi(val) < deadzone)
	{
		return 0;
	}

	return val;
}

vf2 stick_vec_left(ControllerState* cs)
{
	return (vf2) {
		deadzone(cs, AXIS_LEFTX) / AXIS_MAX,
		deadzone(cs, AXIS_LEFTY) / AXIS_MAX
	};
}

vf2 stick_vec_right(ControllerState* cs)
{
	return (vf2) {
		deadzone(cs, AXIS_RIGHTX) / AXIS_MAX,
		deadzone(cs, AXIS_RIGHTY) / AXIS_MAX
	};
}

// FIXME: Write homebrew atan2f to eliminate dependency on SDL
f32 stick_dir_left(ControllerState* cs)
{
	return SDL_atan2f
	(
	 	deadzone(cs, AXIS_LEFTX) / AXIS_MAX,
	 	deadzone(cs, AXIS_LEFTY) / AXIS_MAX
	);
}

f32 stick_dir_right(ControllerState* cs)
{
	return SDL_atan2f
	(
		deadzone(cs, AXIS_RIGHTX) / AXIS_MAX,
		deadzone(cs, AXIS_RIGHTY) / AXIS_MAX);
}

f32 stick_mag_left(ControllerState* cs)
{
    // 1. Normalize axes to [-1,1]
    f32 x = 0.0f;
    f32 y = 0.0f;

    x = deadzone(cs, AXIS_LEFTX) / AXIS_MAX;
    y = deadzone(cs, AXIS_LEFTY) / AXIS_MAX;

    // 2. Compute absolute values
    f32 absX = sl_absf(x);
    f32 absY = sl_absf(y);

    // 3. Polynomial approximation
    f32 minVal = (absX < absY) ? absX : absY;
    f32 magnitude = absX + absY - minVal * ANALOG_STICK_CORRECTION;

    // 4. Clamp to 1.0
    if (magnitude > 1.0f)
        magnitude = 1.0f;

    return magnitude;
}

f32 stick_mag_right(ControllerState* cs)
{
    // 1. Normalize axes to [-1,1]
    f32 x = 0.0f;
    f32 y = 0.0f;

    x = deadzone(cs, AXIS_RIGHTX) / AXIS_MAX;
    y = deadzone(cs, AXIS_RIGHTY) / AXIS_MAX;

    // 2. Compute absolute values
    f32 absX = sl_absf(x);
    f32 absY = sl_absf(y);

    // 3. Polynomial approximation
    f32 minVal = (absX < absY) ? absX : absY;
    f32 magnitude = absX + absY - minVal * ANALOG_STICK_CORRECTION;

    // 4. Clamp to 1.0
    if (magnitude > 1.0f)
        magnitude = 1.0f;

    return magnitude;
}

f32 trigger_mag_left(ControllerState* cs)
{
	return deadzone(cs, AXIS_TRIGGER_L) / AXIS_MAX;
}

f32 trigger_mag_right(ControllerState* cs)
{
	return deadzone(cs, AXIS_TRIGGER_R) / AXIS_MAX;
}

// Keys
b8 key_pressed(KeyboardState* keyboard, KeyboardMap key)
{
	return (keyboard->key[key] == BUTTON_PRESSED);
}

b8 key_held(KeyboardState* keyboard, KeyboardMap key)
{
	return (keyboard->key[key] == BUTTON_HELD);
}

b8 key_released(KeyboardState* keyboard, KeyboardMap key)
{
	return (keyboard->key[key] == BUTTON_RELEASED);
}

b8 key_idle(KeyboardState* keyboard, KeyboardMap key)
{
	return (keyboard->key[key] == BUTTON_IDLE);
}

// Mouse
b8 mouse_button_pressed(MouseState* ms, MouseButtonMap bs)
{
	return (ms->button[bs] == BUTTON_PRESSED);
}

b8 mouse_button_held(MouseState* ms, MouseButtonMap bs)
{
	return (ms->button[bs] == BUTTON_HELD);
}

b8 mouse_button_released(MouseState* ms, MouseButtonMap bs)
{
	return (ms->button[bs] == BUTTON_RELEASED);
}

b8 mouse_button_idle(MouseState* ms, MouseButtonMap bs)
{
	return (ms->button[bs] == BUTTON_IDLE);
}


void game_input(GameInput* game_input) 
{

	ControllerState* cs = &game_input->controller_state;
/*
	// Face Buttons
	if (button_pressed(cs, BUTTON_NORTH)) SDL_Log("North Button Pressed");
	if (button_pressed(cs, BUTTON_SOUTH)) SDL_Log("South Button Pressed");
	if (button_pressed(cs, BUTTON_EAST)) SDL_Log("East Button Pressed");
	if (button_pressed(cs, BUTTON_WEST)) SDL_Log("West Button Pressed");

	// DPAD
	if (button_pressed(cs, DPAD_UP)) SDL_Log("DPAD Up Pressed");
	if (button_pressed(cs, DPAD_DOWN)) SDL_Log("DPAD Down Pressed");
	if (button_pressed(cs, DPAD_LEFT)) SDL_Log("DPAD Left Pressed");
	if (button_pressed(cs, DPAD_RIGHT)) SDL_Log("DPAD Right Pressed");

	// Shoulder
	if (button_pressed(cs, SHOULDER_LEFT)) SDL_Log("Left Shoulder Button Pressed");
	if (button_pressed(cs, SHOULDER_RIGHT)) SDL_Log("Right Shoulder Button Pressed");

	// Triggers
	if (button_pressed(cs, TRIGGER_LEFT)) SDL_Log("Left Trigger Button Pressed");
	if (button_pressed(cs, TRIGGER_RIGHT)) SDL_Log("Right Trigger Button Pressed");

	// Sticks
	if (button_pressed(cs, CLICK_LEFT)) SDL_Log("Left Stick Button Pressed");
	if (button_pressed(cs, CLICK_RIGHT)) SDL_Log("Right Stick Button Pressed");

	// System
	if (button_pressed(cs, BUTTON_START)) SDL_Log("Start Button Pressed");
	if (button_pressed(cs, BUTTON_SELECT)) SDL_Log("Select Button Pressed");

	SDL_Log("Left Stick Vector x: %f", stick_vec_left(cs).x); 
	SDL_Log("Left Stick Vector y: %f", stick_vec_left(cs).y); 

	SDL_Log("Left Stick Dir: %f", stick_dir_left(cs));
	SDL_Log("Left Stick Mag: %f", stick_mag_left(cs));

	SDL_Log("Right Stick Vector x: %f", stick_vec_right(cs).x);
	SDL_Log("Right Stick Vector y: %f", stick_vec_right(cs).y);

	SDL_Log("Right Stick Dir: %f", stick_dir_right(cs));
	SDL_Log("Right Stick Mag: %f", stick_mag_right(cs));

	SDL_Log("Left Trigger Mag: %f", trigger_mag_left(cs));
	SDL_Log("Right Trigger Mag: %f", trigger_mag_right(cs));

	// Controller Axes
	SDL_Log("Axis 0:%i", cs->axis[0]);
	SDL_Log("Axis 1:%i", cs->axis[1]);
	SDL_Log("Axis 2:%i", cs->axis[2]);
	SDL_Log("Axis 3:%i", cs->axis[3]);
	SDL_Log("Axis 4:%i", cs->axis[4]);
	SDL_Log("Axis 5:%i", cs->axis[5]);

	// Keyboard
	KeyboardState* ks = &game_input->keyboard_state;

	if (key_pressed(ks, KEY_0)) SDL_Log("0 key pressed");
	if (key_pressed(ks, KEY_1)) SDL_Log("1 key pressed");
	if (key_pressed(ks, KEY_2)) SDL_Log("2 key pressed");
	if (key_pressed(ks, KEY_3)) SDL_Log("3 key pressed");
	if (key_pressed(ks, KEY_4)) SDL_Log("4 key pressed");
	if (key_pressed(ks, KEY_5)) SDL_Log("5 key pressed");
	if (key_pressed(ks, KEY_6)) SDL_Log("6 key pressed");
	if (key_pressed(ks, KEY_7)) SDL_Log("7 key pressed");
	if (key_pressed(ks, KEY_8)) SDL_Log("8 key pressed");
	if (key_pressed(ks, KEY_9)) SDL_Log("9 key pressed");

	if (key_pressed(ks, KEY_A)) SDL_Log("A key pressed");
	if (key_pressed(ks, KEY_B)) SDL_Log("B key pressed");
	if (key_pressed(ks, KEY_C)) SDL_Log("C key pressed");
	if (key_pressed(ks, KEY_D)) SDL_Log("D key pressed");
	if (key_pressed(ks, KEY_E)) SDL_Log("E key pressed");
	if (key_pressed(ks, KEY_F)) SDL_Log("F key pressed");
	if (key_pressed(ks, KEY_G)) SDL_Log("G key pressed");
	if (key_pressed(ks, KEY_H)) SDL_Log("H key pressed");
	if (key_pressed(ks, KEY_I)) SDL_Log("I key pressed");
	if (key_pressed(ks, KEY_J)) SDL_Log("J key pressed");
	if (key_pressed(ks, KEY_K)) SDL_Log("K key pressed");
	if (key_pressed(ks, KEY_L)) SDL_Log("L key pressed");
	if (key_pressed(ks, KEY_M)) SDL_Log("M key pressed");
	if (key_pressed(ks, KEY_N)) SDL_Log("N key pressed");
	if (key_pressed(ks, KEY_O)) SDL_Log("O key pressed");
	if (key_pressed(ks, KEY_P)) SDL_Log("P key pressed");
	if (key_pressed(ks, KEY_Q)) SDL_Log("Q key pressed");
	if (key_pressed(ks, KEY_R)) SDL_Log("R key pressed");
	if (key_pressed(ks, KEY_S)) SDL_Log("S key pressed");
	if (key_pressed(ks, KEY_T)) SDL_Log("T key pressed");
	if (key_pressed(ks, KEY_U)) SDL_Log("U key pressed");
	if (key_pressed(ks, KEY_V)) SDL_Log("V key pressed");
	if (key_pressed(ks, KEY_W)) SDL_Log("W key pressed");
	if (key_pressed(ks, KEY_X)) SDL_Log("X key pressed");
	if (key_pressed(ks, KEY_Y)) SDL_Log("Y key pressed");
	if (key_pressed(ks, KEY_Z)) SDL_Log("Z key pressed");

	if (key_pressed(ks, KEY_MINUS)) SDL_Log("MINUS key pressed");
	if (key_pressed(ks, KEY_EQUALS)) SDL_Log("EQUALS key pressed");
	if (key_pressed(ks, KEY_LEFTBRACKET)) SDL_Log("LEFTBRACKET key pressed");
	if (key_pressed(ks, KEY_RIGHTBRACKET)) SDL_Log("RIGHTBRACKET key pressed");
	if (key_pressed(ks, KEY_BACKSLASH)) SDL_Log("BACKSLASH key pressed");
	if (key_pressed(ks, KEY_SEMICOLON)) SDL_Log("SEMICOLON key pressed");
	if (key_pressed(ks, KEY_APOSTROPHE)) SDL_Log("APOSTROPHE key pressed");
	if (key_pressed(ks, KEY_GRAVE)) SDL_Log("GRAVE key pressed");
	if (key_pressed(ks, KEY_COMMA)) SDL_Log("COMMA key pressed");
	if (key_pressed(ks, KEY_PERIOD)) SDL_Log("PERIOD key pressed");
	if (key_pressed(ks, KEY_SLASH)) SDL_Log("SLASH key pressed");

	if (key_pressed(ks, KEY_ESCAPE)) SDL_Log("ESCAPE key pressed");
	if (key_pressed(ks, KEY_TAB)) SDL_Log("TAB key pressed");
	if (key_pressed(ks, KEY_SPACE)) SDL_Log("SPACE key pressed");

	if (key_pressed(ks, KEY_F1)) SDL_Log("F1 key pressed");
	if (key_pressed(ks, KEY_F2)) SDL_Log("F2 key pressed");
	if (key_pressed(ks, KEY_F3)) SDL_Log("F3 key pressed");
	if (key_pressed(ks, KEY_F4)) SDL_Log("F4 key pressed");
	if (key_pressed(ks, KEY_F5)) SDL_Log("F5 key pressed");
	if (key_pressed(ks, KEY_F6)) SDL_Log("F6 key pressed");
	if (key_pressed(ks, KEY_F7)) SDL_Log("F7 key pressed");
	if (key_pressed(ks, KEY_F8)) SDL_Log("F8 key pressed");
	if (key_pressed(ks, KEY_F9)) SDL_Log("F9 key pressed");
	if (key_pressed(ks, KEY_F10)) SDL_Log("F10 key pressed");
	if (key_pressed(ks, KEY_F11)) SDL_Log("F11 key pressed");
	if (key_pressed(ks, KEY_F12)) SDL_Log("F12 key pressed");

	// Mouse
	MouseState* ms = &game_input->mouse_state;
	if (mouse_button_pressed(ms, MOUSE_BUTTON_LEFT)) SDL_Log("Left Mouse Button Pressed");
	if (mouse_button_pressed(ms, MOUSE_BUTTON_RIGHT)) SDL_Log("Right Mouse Button Pressed");
	if (mouse_button_pressed(ms, MOUSE_BUTTON_MIDDLE)) SDL_Log("Middle Mouse Button Pressed");
	if (mouse_button_pressed(ms, MOUSE_BUTTON_SIDE_1)) SDL_Log("Side Mouse Button 1 Pressed");
	if (mouse_button_pressed(ms, MOUSE_BUTTON_SIDE_2)) SDL_Log("Side Mouse Button 2 Pressed");

	SDL_Log("Axis Right X Value: %f", ms->pos_x);
	SDL_Log("Axis Right Y Value: %f", ms->pos_y);

	SDL_Log("Axis Right X Relative Value: %f", ms->rel_x);
	SDL_Log("Axis Right Y Relative Value: %f", ms->rel_y);
*/
}
