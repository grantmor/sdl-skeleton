#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_oldnames.h>
#include <stdint.h>

#include "../inc/cglm/struct.h"

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

// Alias cglm types
// typedef vec2s v2f;
typedef struct {
	f32 x;
	f32 y;
} v2f;

typedef v2f Pos2;
typedef v2f Dir2;

// typedef vec3s v3f;
typedef struct {
	f32 x;
	f32 y;
	f32 z;
} v3f;

typedef v3f Pos3;
typedef v3f Dir3;

typedef ivec2s v2i;

// No unsigned structs in cglm
typedef struct {
	u32 x;
	u32 y;
} v2u;
