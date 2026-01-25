#pragma once

#define MAX_RENDER_PHASES 8

#include "SDL3/SDL.h"
#include "types.h"
#include "../inc/cglm/struct.h"

typedef struct {
    u32 x, y; // Atlas offset
    u32 w, h; // Frame size
    i16 px, py; // Pivot Offset
} TileFrame;

// types: orthographic, isometric, projected2d
typedef struct {
	Pos2 pos;
	Dir2 rot;
	v2f zoom;
} Camera2D;

typedef enum {
	ATLAS_BG,
	ATLAS_TILE,
	ATLAS_SPRITE,
	ATLAS_COUNT,
} TextureAtlasKind;

typedef struct {
	SDL_Texture* data;
	SDL_Time modified;
	char* path;
} TextureAtlas;

typedef struct {
	SDL_Renderer* sdl_renderer;
	TextureAtlas atlas[ATLAS_COUNT];
	v2u render_size;
} RenderCtx;

typedef enum {
	RENDER_PHASE_2D_AFFINE,
	RENDER_PHASE_2D_PROJECTED,
	RENDER_PHASE_2D_SHAPES,
	RENDER_PHASE_3D_MODEL,
	RENDER_PHASE_POST,
} RenderPhaseType;

typedef struct {
	// mat3s view;
	Camera2D camera;
	TextureAtlasKind atlas_kind;
	TileFrame* tiles;
	u32* tile_indices;
	v2f* tile_positions;
	u32 tile_index_count;
} AffinePhase2D;

typedef struct {
	mat4 view;
	mat4 proj;
	TextureAtlasKind atlas_kind;
} ProjectedPhase2D;

typedef struct {
	mat3 view;
} ShapePhase;

typedef struct {
	mat4 view;
	mat4 proj;
} ModelPhase;

typedef struct {
	RenderPhaseType type;
	union {
	 	AffinePhase2D affine_2d;
	 	ProjectedPhase2D projected_2d;
	 	ShapePhase shape_2d;
	 	ModelPhase model;
	} render_phase_data;
} RenderPhase;

typedef struct {
	RenderPhase render_phases[MAX_RENDER_PHASES];
	u32 render_phase_count;
} RenderList;

void render_texture_atlas_load(SDL_Renderer* renderer, TextureAtlas* atlas);

void render_clear(RenderCtx* rctx);

void render_atlas(RenderCtx* rctx, TextureAtlasKind atlas_type);

void render_make(SDL_Window* window, v2u fb_size);

void render_frame(RenderList* render_list);

void render_free();

