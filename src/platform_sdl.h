#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "types.h"

#include "audio.h"
#include "input.h"

#include "super_lib.c"

typedef enum {
	TEXT_COLOR_BLACK,
	TEXT_COLOR_RED,
	TEXT_COLOR_GREEN,
	TEXT_COLOR_YELLOW,
	TEXT_COLOR_BLUE,
	TEXT_COLOR_MAGENTA,
	TEXT_COLOR_CYAN,
	TEXT_COLOR_WHITE,
	TEXT_COLOR_COUNT
} TextColor;

typedef enum {
	LOG_TYPE_TRACE,
	LOG_TYPE_INFO,
	LOG_TYPE_WARN,
	LOG_TYPE_ERROR
} LogType;

typedef struct {
	f64 last_time;
	f64 cur_time;
	f64 dt;
	f64 fps_avg;
	u64 frame_counter;
} Time;

typedef struct {
	SDL_Gamepad* platform_gamepad;
	ControllerMap* sdl_to_game[128];

	PlatformControllerState gamepad_state_prev;
	PlatformKeyboardState keyboard_prev;
	PlatformMouseState mouse_state_prev;
} PlatformInput;

typedef struct {
	ControllerState controller_state;
	KeyboardState keyboard_state;
	MouseState mouse_state; 
} GameInput;

typedef struct {
	SDL_Texture* atlas;
	SDL_Time modified;
	char* path;
	
	u32 width;
	u32 height;
} SpriteAtlas;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;

	Time time;

	// Input
	PlatformInput platform_input;
	GameInput game_input;

	// Video
	SpriteAtlas sprite_atlas;

	// Sound
	SoundManager sound_manager;

	// Resources
	// TODO: This obviously shouldn't be naked in global state
	//SoundClip* sound_clip;
} AppState;

// *** File IO ***
i64 platform_file_timestamp_get(char* file);

void platform_file_load();

void platform_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas);

void platform_log(char* message, LogType log_type);

void platform_trace(char* message);

void platform_info(char* message);

void platform_warn(char* message);

void platform_error(char* message);
