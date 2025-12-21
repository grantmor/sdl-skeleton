#pragma once

#include <SDL3/SDL.h>
#include "types.h"

#include "audio.c"
#include "input.h"

#include "super_lib.c"

typedef struct {
	f64 last_time;
	f64 cur_time;
	f64 dt;
	f64 fps_avg;
	u64 frame_counter;
} Time;

typedef struct {
	SDL_Gamepad* platform_gamepad;

	PlatformControllerState gamepad_state_prev;
	PlatformKeyboardState keyboard_prev;
	PlatformMouseState mouse_state_prev;

	ControllerMap* sdl_to_game[128];
} PlatformInput;

typedef struct {
	ControllerState controller_state;
	KeyboardState keyboard_state;
	MouseState mouse_state; 
} GameInput;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;

	Time time;

	// Input
	PlatformInput platform_input;
	GameInput game_input;

	// Sound
	SoundManager sound_manager;

	// Resources
	// TODO: This obviously shouldn't be naked in global state
	SoundClip* sound_clip;
	
} AppState;
