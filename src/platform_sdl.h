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

/*
typedef struct {
	SpriteAtlas sprite_atlas;	
} ImageManager;
*/

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
	SoundClip* sound_clip;
} AppState;
void platform_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas)
{
	if (atlas->atlas)
	{
		SDL_DestroyTexture(atlas->atlas);
	}

	SDL_Surface* sprite_atlas_surf = SDL_LoadBMP(atlas->path);
	SDL_Texture* sprite_atlas_tex = SDL_CreateTextureFromSurface(renderer, sprite_atlas_surf);
	if (!sprite_atlas_tex) {
	    SDL_Log("Failed to create texture: %s", SDL_GetError());
	}
	else
	{
		atlas->atlas = sprite_atlas_tex;
		atlas->modified = file_get_timestamp(atlas->path);
		SDL_DestroySurface(sprite_atlas_surf);
	}
}
