#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "platform_sdl.h"
#include "platform_iterate.c"
#include "platform_init.c"
#include "platform_event.c"
#include "platform_quit.c"

//FIXME: Swap out (most) SDL_Log()s for SDL_LogDebug()s

i64 platform_file_timestamp_get(char* file)
{
	SDL_PathInfo path_info;
	if (!SDL_GetPathInfo(file, &path_info))
	{
		#ifndef __EMSCRIPTEN__
		SDL_Log("Could not get timestamp for file!");	
		#endif
		return -1;
	}

	return path_info.modify_time;
}

void platform_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas)
{
	SDL_Log("atlas path: %s", atlas->path);
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
		atlas->modified = platform_file_timestamp_get(atlas->path);
		SDL_DestroySurface(sprite_atlas_surf);
		SDL_SetTextureScaleMode(atlas->atlas, SDL_SCALEMODE_NEAREST);
	}
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[])
{
	return platform_init(appstate);
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return platform_event( (AppState*) appstate, event);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	return platform_iterate( (AppState*) appstate);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	platform_quit( (AppState*) appstate);
}
