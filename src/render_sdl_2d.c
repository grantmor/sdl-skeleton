#include "SDL3/SDL.h"
#include "render_sdl_2d.h"
#include "platform_sdl.h"
#include <SDL3/SDL_render.h>

static Renderer g_renderer;

void render_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas)
{
	if (!g_platform_api) {
        ERROR("FATAL: g_platform is NULL!\n");
        return;
    }

    if (!atlas || !atlas->path) {
        ERROR("FATAL: atlas or atlas->path is NULL!\n");
        return;
    }

	INFO("atlas path: %s", atlas->path);
	if (atlas->atlas)
	{
		SDL_DestroyTexture(atlas->atlas);
	}

	SDL_Surface* sprite_atlas_surf = SDL_LoadBMP(atlas->path);
	SDL_Texture* sprite_atlas_tex = SDL_CreateTextureFromSurface(renderer, sprite_atlas_surf);
	if (!sprite_atlas_tex) {
	    WARN("Failed to create texture: %s", SDL_GetError());
	}
	else
	{
		atlas->atlas = sprite_atlas_tex;
		atlas->modified = platform_file_timestamp_get(atlas->path);
		SDL_DestroySurface(sprite_atlas_surf);
		SDL_SetTextureScaleMode(atlas->atlas, SDL_SCALEMODE_NEAREST);
	}
}

void render_init(SDL_Window* window)
{
	g_renderer = (Renderer){0};
	g_renderer.sdl_renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderLogicalPresentation(g_renderer.sdl_renderer, 640, 360, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	// Textures
	g_renderer.sprite_atlas.path = "res/image/sprites.bmp";
	render_sprite_atlas_load(g_renderer.sdl_renderer, &g_renderer.sprite_atlas);
}

void platform_render(Renderer* renderer)
{
	// SDL_Renderer* renderer = as->renderer;
	SpriteAtlas* sprite_atlas = &renderer->sprite_atlas;

	if (platform_file_timestamp_get(sprite_atlas->path) > sprite_atlas->modified)
	{
		render_sprite_atlas_load(g_renderer.sdl_renderer, sprite_atlas);
	}

	SDL_SetRenderDrawColorFloat(g_renderer.sdl_renderer, 0.0,0.0,0.0,1.0);
	SDL_RenderClear(renderer->sdl_renderer);

	// Layer 0 - Just here to visualize screen area and letterboxing
	SDL_SetRenderDrawColor(renderer->sdl_renderer, 0.0, 0.0, 255, 255);
	SDL_FRect rect = (SDL_FRect) {0.0,0.0,320.0,180.0};
	SDL_RenderFillRect(renderer->sdl_renderer, &rect);

	// Layer 1 - Testing Sprite Atlas
	SDL_FRect sprite_rect = (SDL_FRect) {0.0,0.0,0.0,0.0};
	SDL_GetTextureSize(sprite_atlas->atlas, &sprite_rect.w, &sprite_rect.h);
	SDL_RenderTexture(renderer->sdl_renderer, sprite_atlas->atlas, NULL, &sprite_rect);

	SDL_RenderPresent(renderer->sdl_renderer);
}


void render_frame()
{
	platform_render(&g_renderer);
}

void render_free()
{
	SDL_DestroyTexture(g_renderer.sprite_atlas.atlas);
	SDL_DestroyRenderer(g_renderer.sdl_renderer);
}
