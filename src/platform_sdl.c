#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "platform_sdl.h"


#include "platform_iterate.c"
#include "platform_init.c"
#include "platform_event.c"
#include "platform_quit.c"
 
 void platform_log(char* message, LogType log_type)
{
	static char* color_table[TEXT_COLOR_COUNT] =
	{
		"\x1b[30m", // TEXT_COLOR_BLACK
		"\x1b[31m", // TEXT_COLOR_RED
		"\x1b[32m", // TEXT_COLOR_GREEN
		"\x1b[33m", // TEXT_COLOR_YELLOW
		"\x1b[34m", // TEXT_COLOR_BLUE
		"\x1b[35m", // TEXT_COLOR_MAGENTA
		"\x1b[36m", // TEXT_COLOR_CYAN
		"\x1b[37m" // TEXT_COLOR_WHITE
	};

	char* text_color_code;
	char* log_type_text;
	char* text_color_white = color_table[TEXT_COLOR_WHITE];
	char* new_line = "\n";

	switch (log_type)
	{
		case LOG_TYPE_TRACE:
			text_color_code = color_table[TEXT_COLOR_WHITE];		
			log_type_text = "TRACE: ";
			break;
		case LOG_TYPE_INFO:
			text_color_code = color_table[TEXT_COLOR_GREEN];		
			log_type_text = "INFO: ";
			break;
		case LOG_TYPE_WARN:
			text_color_code = color_table[TEXT_COLOR_YELLOW];		
			log_type_text = "WARNING: ";
			break;
		case LOG_TYPE_ERROR:
			text_color_code = color_table[TEXT_COLOR_RED];		
			log_type_text = "ERROR: ";
			break;
	}
	
	// SDL_Log("%s%s%s%s \033[0m", text_color_code, log_type_text, message, new_line);
	SDL_Log("%s%s%s%s%s \033[0m", text_color_code, log_type_text, text_color_white, message, new_line);
}

// FIXME: Accepting dependence on libc for a while to get this thing ready to use
void platform_trace(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    platform_log(buf, LOG_TYPE_TRACE);
}

void platform_info(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    platform_log(buf, LOG_TYPE_INFO);
}

void platform_warn(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    platform_log(buf, LOG_TYPE_WARN);
}

void platform_error(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    platform_log(buf, LOG_TYPE_ERROR);
}


i64 platform_file_timestamp_get(char* file)
{
	SDL_PathInfo path_info;
	if (!SDL_GetPathInfo(file, &path_info))
	{
		#ifndef __EMSCRIPTEN__
		platform_trace("Could not get timestamp for file because we're on the web!");	
		#endif
		return -1;
	}

	return path_info.modify_time;
}

void platform_sprite_atlas_load(SDL_Renderer* renderer, SpriteAtlas* atlas)
{
	INFO("atlas path: %s", atlas->path);
	if (atlas->atlas)
	{
		SDL_DestroyTexture(atlas->atlas);
	}

	SDL_Surface* sprite_atlas_surf = SDL_LoadBMP(atlas->path);
	SDL_Texture* sprite_atlas_tex = SDL_CreateTextureFromSurface(renderer, sprite_atlas_surf);
	if (!sprite_atlas_tex) {
	    ERROR("Failed to create texture: %s", SDL_GetError());
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
