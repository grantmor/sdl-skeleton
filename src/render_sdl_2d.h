#pragma once

#define MAX_RENDER_PHASES 8

#include "SDL3/SDL.h"
#include "types.h"

typedef struct {
	SDL_Texture* data;
	SDL_Time modified;
	char* path;
} TextureAtlas;

typedef enum {
	ATLAS_BG,
	ATLAS_TILE,
	ATLAS_SPRITE,
	ATLAS_COUNT,
} AtlasType;

typedef struct {
	SDL_Renderer* sdl_renderer;
	TextureAtlas atlas[ATLAS_COUNT];
	v2u render_size;
} RenderCtx;

typedef enum {
	RENDER_PHASE_TILE,	
	RENDER_PHASE_SPRITE,
	RENDER_PHASE_SHAPE,
} RenderPhaseType;

typedef struct {
	
} TileRenderPhase;

typedef struct {
	
} SpriteRenderPhase;

typedef struct {
	
} ShapeRenderPhase;

typedef struct {
	RenderPhaseType type;
	union {
		TileRenderPhase tiles;
	 	SpriteRenderPhase sprites;
	 	ShapeRenderPhase shapes;
	} RenderPhaseData;
} RenderPhase;

typedef struct {
	RenderPhase render_phases[MAX_RENDER_PHASES];
	u32 render_phase_count;
} RenderList;

void render_texture_atlas_load(SDL_Renderer* renderer, TextureAtlas* atlas);

void render_clear(RenderCtx* rctx);

void render_atlas(RenderCtx* rctx, AtlasType atlas_type);

void render_init(SDL_Window* window);

void render_frame(RenderList* render_list);

void render_free();

