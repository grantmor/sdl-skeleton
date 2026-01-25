#include "SDL3/SDL.h"
#include "render_sdl_2d.h"
#include "platform_sdl.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "types.h"

static RenderCtx g_rctx;

void render_texture_atlas_load(SDL_Renderer* renderer, TextureAtlas* atlas)
{
	if (!g_platform_api) {
        ERROR("FATAL: g_platform is NULL!\n");
        return;
    }

    if (!atlas || !atlas->path) {
        ERROR("FATAL: atlas or atlas->path is NULL!\n");
        return;
    }

	if (atlas->data)
	{
		SDL_DestroyTexture(atlas->data);
	}

	SDL_Surface* sprite_atlas_surf = SDL_LoadBMP(atlas->path);
	SDL_Texture* sprite_atlas_tex = SDL_CreateTextureFromSurface(renderer, sprite_atlas_surf);

	if (!sprite_atlas_tex) {
	    WARN("Failed to create texture: %s", SDL_GetError());
	}
	else
	{
		atlas->data = sprite_atlas_tex;
		atlas->modified = platform_file_timestamp_get(atlas->path);
		SDL_DestroySurface(sprite_atlas_surf);
		SDL_SetTextureScaleMode(atlas->data, SDL_SCALEMODE_NEAREST);

		INFO("%s loaded", atlas->path);
	}
}

void render_make(SDL_Window* window, v2u fb_size)
{
	g_rctx = (RenderCtx) {0};
	g_rctx.sdl_renderer = SDL_CreateRenderer(window, NULL);
	g_rctx.render_size = fb_size;

	SDL_SetRenderLogicalPresentation(g_rctx.sdl_renderer, fb_size.x, fb_size.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	for (usize a=0; a<ATLAS_COUNT; a++)
	{
		switch (a)
		{
			case ATLAS_BG:
				g_rctx.atlas[a].path = "res/image/bg.bmp";
				break;
			case ATLAS_TILE:
				g_rctx.atlas[a].path = "res/image/tile.bmp";
				break;
			case ATLAS_SPRITE:
				g_rctx.atlas[a].path = "res/image/sprite.bmp";
				break;
		}

		render_texture_atlas_load(g_rctx.sdl_renderer, &g_rctx.atlas[a]);
	}
}

void render_clear(RenderCtx* rctx)
{
	SDL_Renderer* sdl_ren = rctx->sdl_renderer;
	SDL_SetRenderDrawColorFloat(sdl_ren, 0.0,0.0,0.0,1.0);
	SDL_RenderClear(sdl_ren);

	// Size of the framebuffer to visualize letterboxing
	SDL_SetRenderDrawColorFloat(sdl_ren, 0.0, 0.0, 1.0, 1.0);
	SDL_FRect screen_rect = (SDL_FRect) {0.0,0.0, rctx->render_size.x, rctx->render_size.y};
	SDL_RenderFillRect(sdl_ren, &screen_rect);
}

void render_atlas(RenderCtx* rctx, TextureAtlasKind atlas_type)
{
	SDL_Renderer* sdl_ren = rctx->sdl_renderer;
	TextureAtlas* sprite_atlas = &rctx->atlas[atlas_type];

	if (platform_file_timestamp_get(sprite_atlas->path) > sprite_atlas->modified)
	{
		render_texture_atlas_load(sdl_ren, sprite_atlas);
	}

	SDL_FRect sprite_rect = (SDL_FRect) {0.0,0.0,0.0,0.0};
	SDL_GetTextureSize(sprite_atlas->data, &sprite_rect.w, &sprite_rect.h);
	SDL_RenderTexture(sdl_ren, sprite_atlas->data, NULL, &sprite_rect);
}

void render_tile(RenderCtx* rctx, TextureAtlasKind atlas_type, TileFrame frame, v2f loc)
{
	SDL_Renderer* sdl_ren = rctx->sdl_renderer;
	TextureAtlas* sprite_atlas = &rctx->atlas[atlas_type];

	if (platform_file_timestamp_get(sprite_atlas->path) > sprite_atlas->modified)
	{
		render_texture_atlas_load(sdl_ren, sprite_atlas);
	}

	SDL_FRect sprite_rect = (SDL_FRect) {frame.x, frame.y, frame.w, frame.h};
	SDL_FRect dest_rect = (SDL_FRect) {loc.x - frame.px, loc.y - frame.py, frame.w, frame.h};
	SDL_RenderTexture(sdl_ren, sprite_atlas->data, &sprite_rect, &dest_rect);
}

void render_render_list(RenderCtx* rctx, RenderList* render_list)
{
	for (usize p=0; p<render_list->render_phase_count; p++)
	{
		RenderPhase phase = render_list->render_phases[p];
		switch(phase.type)
		{
			case RENDER_PHASE_2D_AFFINE:
			{
				// RenderPhase affine_phase = phase.render_phase_data.affine_2d;
				for (usize t=0; t<phase.render_phase_data.affine_2d.tile_index_count; t++ )
				{
					render_tile
					(
						rctx,
						phase.render_phase_data.affine_2d.atlas_kind,
						phase.render_phase_data.affine_2d.tiles[phase.render_phase_data.affine_2d.tile_indices[t]],
						phase.render_phase_data.affine_2d.tile_positions[t]
					);	
				}
				break;	
			}
			default:
				break;
		}
	}
}

void render_frame(RenderList* render_list)
{
	render_clear(&g_rctx);
	render_atlas(&g_rctx, ATLAS_TILE);
	render_render_list(&g_rctx, render_list);
	SDL_RenderPresent(g_rctx.sdl_renderer);
}

void render_free()
{
	for (usize a=0; a<ATLAS_COUNT; a++)
	{
		SDL_DestroyTexture(g_rctx.atlas[a].data);
	}

	SDL_DestroyRenderer(g_rctx.sdl_renderer);
}
