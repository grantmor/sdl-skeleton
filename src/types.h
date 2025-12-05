#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} AppState;
