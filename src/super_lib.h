#pragma once
#include "types.h"

// *** Memory ***
typedef struct {
	usize capacity;
	usize used;
	u8* data;
} Arena;

typedef struct {
	Arena* arena;
	usize offset;
} SubArena;

typedef struct {
	Arena* arena;
	Arena* scratch;
} MemoryContext;

Arena arena_alloc_create(u8* backing_buffer, usize size);

u8* arena_alloc(Arena* alloc, usize size);

Arena* arena_reset(Arena* arena);

void arena_free(Arena* arena);

SubArena arena_sub_start(Arena* arena);

void arena_sub_end(SubArena sub_arena);

// FIXME: Placeholders, make these fast (probably branchless)
// Math
i64 absi(i64 i);
f32 absf(f32 f);
i64 mini(i64 a, i64 b);
i64 maxi(i64 a, i64 b);

// Strings
typedef struct {
	u8* text;
	usize length;
} String;

String str_alloc(Arena* arena, usize length);

String str_create_size(MemoryContext* mctx, c8* cstring, usize length);

String str_create(MemoryContext* mctx, c8* cstring);

String str_concat(MemoryContext* mctx, String str_a, String str_b);

c8* str_to_cstring(MemoryContext* mctx, String str);

c8* str_put_2_str(MemoryContext* mctx, String a, String b);

c8* str_put_2_c(MemoryContext* mctx, c8* a, c8* b);
