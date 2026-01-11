#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_oldnames.h>
#include <stdint.h>


typedef Uint8 u8;
typedef Uint16 u16;
typedef Uint32 u32;
typedef Uint64 u64;

typedef Sint8 i8;
typedef Sint16 i16;
typedef Sint32 i32;
typedef Sint64 i64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef char c8;

typedef size_t usize;

#define KB(x) ((usize) 1024 * x)
#define MB(x) ((usize) 1024 * KB(x))
#define GB(x) ((usize) 1024 * MB(x))

typedef struct {
	f32 x;
	f32 y;	
} vf2;
