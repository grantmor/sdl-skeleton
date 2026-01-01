#pragma once

#include <SDL3/SDL_filesystem.h>
#define FREE_VAL 0xFF

#include <SDL3/SDL.h>
#include <SDL3/SDL_assert.h>
#include "types.h"

// *** Memory ***

typedef struct {
	usize capacity;
	usize used;
	u8* data;
} ArenaAllocator;

ArenaAllocator arena_alloc_create(usize size)
{
	u8* data = SDL_calloc(1, size);
	if (!data)
	{
		SDL_Log("Attempted memory allocation failed!");
	}

	return (ArenaAllocator)
	{
		.used = 0,
		.capacity = size,
		.data = data	
	};	
}

u8* arena_alloc(ArenaAllocator* alloc, usize size)
{
	u8* result = NULL;

	// Ensure memory is aligned to 16 bytes
	// FIXME: actually 8 bytes?
	usize aligned_size = (size + 7) & ~ 7;
	if (alloc->used + aligned_size <= alloc->capacity)
	{
		result = alloc->data + alloc->used;
		alloc->used += aligned_size;
	}
	else
	{
		SDL_Log("Allocator is full!");
		return NULL;
	}

	return result;
	
}

ArenaAllocator* arena_reset(ArenaAllocator* arena)
{
	for (usize b=0; b<arena->used; b++)
	{
		arena->data[b] = FREE_VAL; 
	}

	arena->used = 0;
	return arena;	
}

void arena_free(ArenaAllocator* arena)
{
	SDL_free(arena->data);
}

// *** File IO ***
i64 file_get_timestamp(char* file)
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
