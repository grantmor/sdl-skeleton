#define ARENA_INIT_VAL 0xFF

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_assert.h>
#include "types.h"
#include "super_lib.h"
// #include "platform_sdl.h"

Arena arena_alloc_make(u8* backing_buffer, usize size)
{
	// FIXME: WARN or ERROR if 
	for (usize b=0; b<size; b++)
	{
		backing_buffer[b] = ARENA_INIT_VAL;
	}

	return (Arena)
	{
		.used = 0,
		.capacity = size,
		.data = backing_buffer	
	};	
}

u8* arena_alloc(Arena* alloc, usize size)
{
	u8* result = NULL;

	// Ensure memory is aligned to 16 bytes
	usize aligned_size = (size + 7) & ~ 7;
	if (alloc->used + aligned_size <= alloc->capacity)
	{
		result = alloc->data + alloc->used;
		alloc->used += aligned_size;
	}
	else
	{
		SDL_Log("Allocator is full!");
		// platform_error("Allocator is full!");
		return NULL;
	}

	return result;
}

Arena* arena_reset(Arena* arena)
{
	arena->used = 0;
	return arena;	
}

void arena_free(Arena* arena)
{
	SDL_free(arena->data);
}

SubArena arena_sub_start(Arena* arena)
{
	return (SubArena) {
		.arena = arena,	
		.offset = arena->used
	};
}

void arena_sub_end(SubArena sub_arena)
{
	sub_arena.arena->used = sub_arena.offset;
}

String str_alloc(Arena* arena, usize length)
{
	return (String)
	{
		.text = arena_alloc(arena, length),
		.length = length
	};	
}

String str_make_size(MemoryContext* mctx, c8* cstring, usize length)
{
	String str = str_alloc(mctx->arena, length);
	memcpy(str.text, cstring, length);
	str.length = length;

	return str;
}

String str_make(MemoryContext* mctx, c8* cstring)
{
	usize length = 0;
	c8 c = cstring[0];	

	while (c != '\0')
	{
		length += 1;
		c = cstring[length];
	}

	return str_make_size(mctx, cstring, length);
}

String str_concat(MemoryContext* mctx, String str_a, String str_b)
{
	u64 length = str_a.length + str_b.length + 1;

	c8 cstring[length];

	memcpy(&cstring, str_a.text, str_a.length);
	memcpy(&cstring[str_a.length], str_b.text, str_b.length);
	cstring[length-1] = '\0';

	String str_c = str_make(mctx, cstring);
	return str_c;
}

c8* str_to_cstring(MemoryContext* mctx, String str)
{
	c8 cstr[str.length+1];

	for (usize c=0; c<str.length; c++)
	{
		cstr[c] = str.text[c];
	}
	cstr[str.length] = '\0';

	u8* mem_ptr = arena_alloc(mctx->arena, str.length+1);
	memcpy(mem_ptr, cstr, str.length+1);

	return (c8*) mem_ptr;
}

c8* str_put_2_str(MemoryContext* mctx, String a, String b)
{
	return str_to_cstring(mctx, str_concat(mctx, a, b));
}

c8* str_put_2_c(MemoryContext* mctx, c8* a, c8* b)
{
	return str_to_cstring(mctx, str_concat(mctx, str_make(mctx, a), str_make(mctx, b)));
}

String str_put(MemoryContext* mctx, u32 count, ...)
{
	SubArena scratch = arena_sub_start(mctx->scratch);

	va_list args;
	u32 char_count = 0;

	va_start (args, count);		
	for (usize c=0; c<count; c++)
	{
		//FIXME: replace strlen()
		char_count += strlen(va_arg(args, const char*));
	}
	va_end(args);

	u8* temp = arena_alloc(mctx->scratch, char_count);

	va_start(args, count);
	usize offset = 0;

	for (usize a=0; a<count; a++)
	{
		const char* string = va_arg(args, const char*);
		usize len = strlen(string);
		memcpy(temp + offset, string, len);
		offset += len;
	}
	va_end(args);

	String result = str_alloc(mctx->arena, char_count);
	memcpy(result.text, temp, char_count);
	result.length = char_count;

	arena_sub_end(scratch);
	// SDL_Log(str_to_cstring(mctx, result));
	return result;
}

// FIXME: rewrite this monstrosity later
c8* str_fmt_c(MemoryContext* mctx, const c8* fmt, ...)
{
    c8 temp[8192];
    usize out = 0;

    va_list args;
    va_start(args, fmt);

    for (usize i = 0; fmt[i] != '\0' && out < sizeof(temp) - 1; i++)
    {
        if (fmt[i] != '%')
        {
            temp[out++] = fmt[i];
            continue;
        }

        // %%
        if (fmt[i + 1] == '%')
        {
            temp[out++] = '%';
            i++;
            continue;
        }

        switch (fmt[++i])
        {
            // C-string
            case 'c':
            {
                const c8* s = va_arg(args, const c8*);
                if (!s) s = "(null)";

                while (*s && out < sizeof(temp) - 1)
                    temp[out++] = *s++;
            } break;

            // custom String
            case 's':
            {
                String str = va_arg(args, String);

                usize copy_len = str.length;
                if (out + copy_len >= sizeof(temp))
                    copy_len = sizeof(temp) - out - 1;

                memcpy(temp + out, str.text, copy_len);
                out += copy_len;
            } break;

            // integer
            case 'i':
            {
                out += snprintf(
                    temp + out,
                    sizeof(temp) - out,
                    "%d",
                    va_arg(args, int)
                );
            } break;

            // float
            case 'f':
            {
                out += snprintf(
                    temp + out,
                    sizeof(temp) - out,
                    "%f",
                    va_arg(args, double)
                );
            } break;

            // fixed-point 16.16
            case 'x':
            {
                int32_t v = va_arg(args, int32_t);
                double f = (double)v / 65536.0;

                out += snprintf(
                    temp + out,
                    sizeof(temp) - out,
                    "%.4f",
                    f
                );
            } break;

            default:
            {
                // unknown specifier → copy literally
                temp[out++] = fmt[i];
            } break;
        }
    }

    va_end(args);

    temp[out] = '\0';

    u8* result = arena_alloc(mctx->arena, out + 1);
    memcpy(result, temp, out + 1);
    return (c8*) result;
}

// FIXME: Placeholders, make these fast (probably branchless)
// Math
i64 absi(i64 i)
{
	if (i>0) return i;
	if (i<0) return i * -1;
	return 0;
}

f32 absf(f32 f)
{
	// FIXME: Need to look into float innacuracy here
	if (f>0) return f;
	if (f<0) return f * -1;
	return 0;
}

i64 mini(i64 a, i64 b)
{
	if (a < b) return a;
	if (b < a) return b;
	return a;
}

i64 maxi(i64 a, i64 b)
{
	if (a > b) return a;
	if (b > a) return b;
	return a;
}
