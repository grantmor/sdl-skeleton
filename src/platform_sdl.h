#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "types.h"

#include "audio.h"
#include "input.h"

#include "super_lib.h"
// #ifdef
#include "render_sdl_2d.h"
#include "entity.h"
// #include "super_lib.c"
// 
#define ARENA_APP_SIZE MB(8)
#define SCRATCH_APP_SIZE MB(8)

#define ARENA_GAME_SIZE MB(8)
#define SCRATCH_GAME_SIZE MB(8)

#define ARENA_SCENE_SIZE MB(8)
#define SCRATCH_SCENE_SIZE MB(8)

#define ARENA_FRAME_SIZE MB(8)
#define SCRATCH_FRAME_SIZE MB(8)

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
	Arena app_arena;
	Arena app_scratch;

	u8 app_arena_buffer[ARENA_APP_SIZE]; 
	u8 app_scratch_buffer[SCRATCH_APP_SIZE]; 
} Memory;

// These pointers should probably be typed...?
typedef struct {
	Arena game_arena;
	Arena game_scratch;

	u8 game_arena_buffer[ARENA_GAME_SIZE];
	u8 game_scratch_buffer[SCRATCH_GAME_SIZE];

	EntityStore entity_store;

	// Other things like scene graph, mode stack, etc.
	// Make sure to clear these pointers when the game arena is reset!!!
	// That goes for any other lifetime/state referenced here
} GameState;

typedef struct {
	Arena frame_arena;
	Arena frame_scratch;
	
	u8 frame_arena_buffer[ARENA_FRAME_SIZE]; 
	u8 frame_scratch_buffer[SCRATCH_FRAME_SIZE];
} FrameState;

typedef struct {
	void (*platform_trace_ptr) (const char* fmt, ...);
	void (*platform_info_ptr) (const char* fmt, ...);
	void (*platform_warn_ptr) (const char* fmt, ...);
	void (*platform_error_ptr) (const char* fmt, ...);
	void (*render_frame_ptr) (RenderList* render_list);
} PlatformAPI;

typedef struct {
	TileFrame* tile_frames;
	SpriteAnimationKind sprite_animation_map[SPRITE_KIND_COUNT][SPRITE_ANIMATION_CLASS_COUNT];
	SpriteAnimation animations[MAX_ANIMATIONS];
} Video;

typedef struct {
	SDL_Window* window;

	Time time;

	// Input
	PlatformInput platform_input;
	GameInput game_input;

	// Video
	// Render render;
	Video video;
	
	// Sound
	SoundManager sound_manager;

	// Memory
	Memory memory;

	// Pointer Table
	PlatformAPI platform_api;

	// Game State
	GameState game_state;

	// Frame State
	FrameState frame_state;

} AppState;

static PlatformAPI* g_platform_api;

// *** File IO ***
i64 platform_file_timestamp_get(char* file);

void platform_file_load();

void platform_log(char* message, LogType log_type);

void platform_trace(const char* fmt, ...);

void platform_info(const char* fmt, ...);

void platform_warn(const char* fmt, ...);

void platform_error(const char* fmt, ...);

#ifdef __EMSCRIPTEN__
	// On the web, disable logging
	#define TRACE(...) ((void)0)
	#define INFO(...)  ((void)0)
	#define WARN(...)  ((void)0)
	#define ERROR(...) ((void)0)
	#define ASSERT(...) ((void)0)
#else
	#define TRACE(...) g_platform_api->platform_trace_ptr(__VA_ARGS__)
	#define INFO(...)  g_platform_api->platform_info_ptr(__VA_ARGS__)
	#define WARN(...)  g_platform_api->platform_warn_ptr(__VA_ARGS__)
	#define ERROR(...) g_platform_api->platform_error_ptr(__VA_ARGS__)

	#define ASSERT(x, ...) \
	{ \
		if (!(x)) \
		{ \
			ERROR(__VA_ARGS__); \
			__builtin_debugtrap(); \
		} \
	}
#endif
