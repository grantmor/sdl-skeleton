#pragma once

#include "SDL3/SDL.h"
#include "platform_sdl.h"

typedef struct {
	SDL_Texture* atlas;
	SDL_Time modified;
	char* path;
	
	u32 width;
	u32 height;
} SpriteAtlas;

typedef struct {
	SDL_Renderer* sdl_renderer;
	SpriteAtlas sprite_atlas;
} Renderer;

void render_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas);

void platform_render(Renderer* renderer);

void render_init(SDL_Window* window);

void render_frame();

void render_free();
