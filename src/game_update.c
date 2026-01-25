#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

#include "platform_sdl.h"

// Replace with render_common.h
#include "render_sdl_2d.h"
#include "super_lib.h"
#include "types.h"

static PlatformAPI* g_platform = NULL;

#ifdef __EMSCRIPTEN__
// On the web, disable logging
#define TRACE(...) ((void)0)
#define INFO(...)  ((void)0)
#define WARN(...)  ((void)0)
#define ERROR(...) ((void)0)
#else
// redefine TRACE macros for the .so to use local pointer
#undef TRACE
#undef INFO
#undef WARN
#undef ERROR

#define TRACE(...) g_platform->platform_trace_ptr(__VA_ARGS__)
#define INFO(...)  g_platform->platform_info_ptr(__VA_ARGS__)
#define WARN(...)  g_platform->platform_warn_ptr(__VA_ARGS__)
#define ERROR(...) g_platform->platform_error_ptr(__VA_ARGS__)

#include "super_lib.c"
#endif

#include "audio.h"
#include "input.h"
#include "input.c"

#include "game_update.h"
#include "entity.h"

static bool g_game_initialized = false;

void game_init(PlatformAPI* platform) {
    g_platform = platform;
	// INFO("Render frame ptr: %p", platform->render_frame_ptr);
}

void entity_player_make()
{
    
}

// Temporary for testing things
// FIXME: PASS IN ENTITY STORE
EntityHandle entity_make(AppState* as)
{
    GameState* gs = &as->game_state;
    EntityStore* es = &as->game_state.entity_store;
    MemoryContext mctx = {&gs->game_arena, &gs->game_scratch};

    SpriteAnimationState anim_state = (SpriteAnimationState) {
        .frame = 0,
        .time_elapsed = 0.0
    };

    Sprite sprite = (Sprite) {
        .sprite_kind = MAIN,
        .anim_state = anim_state,
        .current_animation = MAIN_IDLE
    };

    ASSERT(es->entity_count <= MAX_ENTITIES, "Exceeded max Entity count!!!");

    es->entities[es->entity_count] = (Entity) {
       .dir = {0.0, 0.0},            
       .pos = {160.0, 90.0},
       .sprite = sprite,
    };

    es->entity_count += 1; 

    return (EntityHandle) {.index = es->entity_count - 1, .generation = 0};
}

void game_setup(AppState* as)
{
    MemoryContext mctx = {&as->game_state.game_arena, &as->game_state.game_scratch};
    EntityStore* es = &as->game_state.entity_store;
    Video* video = &as->video;

    // Setup Eneity List
    es->entities = (Entity*) arena_alloc(mctx.arena, sizeof(Entity) * MAX_ENTITIES);
    
    // Thrown away for now
    EntityHandle main = entity_make(as);
}

RenderList* render_list_build(AppState* as)
{
    MemoryContext mctx = (MemoryContext) {.arena = &as->frame_state.frame_arena, .scratch = &as->frame_state.frame_scratch};
    EntityStore* es = &as->game_state.entity_store;
    Video* video = &as->video;

    RenderList* rl = (RenderList*) arena_alloc(mctx.arena, sizeof(RenderList));
    u32* tile_indices = (u32*) arena_alloc(mctx.arena, sizeof(u32) * es->entity_count);
    v2f* tile_positions = (v2f*) arena_alloc(mctx.arena, sizeof(v2f) * es->entity_count);


    // Entities
    for (usize t=0; t<es->entity_count; t++)
    {
        tile_indices[t] = 0;// FIXME: Can't finish this until animation is done
        tile_positions[t] = (v2f) es->entities[t].pos; // Handle shouldn't be needed unless dead entities aren't cleaned up
    }

    // TODO: Follow with other types of things that might need to render a tile
    // UI element for example, if that's a different type

    // Create Tile list
	*rl = (RenderList)
	{
        .render_phase_count = 1,
        .render_phases =
        {
            (RenderPhase)
            {
                .type = RENDER_PHASE_2D_AFFINE,
                .render_phase_data = (AffinePhase2D)
                {
                    .camera = (Camera2D)
                    {
                        .pos = 0,0,
                        .rot = (Dir2) {0.0, 0.0},
                        .zoom = (v2f) {1.0, 1.0},
                    },
                    .atlas_kind = ATLAS_SPRITE,
                    .tiles = video->tile_frames,
                    .tile_indices = tile_indices,
                    .tile_positions = tile_positions,
                    .tile_index_count = es->entity_count,
                }
            } 
        }, 
    };

   return rl; 
}

void game_update(AppState* as)
{
    ASSERT(g_platform != NULL, "g_platform is NULL!");
    if (!g_game_initialized)
    {
        game_setup(as);
        g_game_initialized = true;
    }
	as->platform_api.render_frame_ptr(render_list_build(as));
}

void sound_list_add(SoundManager* sound_man, SoundID sound_id)
{
    sound_man->playing_sounds[sound_man->num_sounds_to_play++] = sound_id;
    // sound_man->num_sounds_to_play += 1;
}

void sound_list_clear(SoundManager* sound_man)
{
    for (i32 s = 0; s < sound_man->num_sounds_to_play; s++)
    {
        sound_man->playing_sounds[s] = SFX_NO_SOUND;
    }
    sound_man->num_sounds_to_play = 0;    
}

void audio_list_build(AppState* as, GameInput* game_input)
{
    SoundManager* sound_man = &as->sound_manager;	
    KeyboardState* ks = &game_input->keyboard_state;

	// Test Audio
	if (key_pressed(ks, KEY_Q)) 
	{
		sound_list_add(sound_man, SFX_COIN);
	}

	if (key_pressed(ks, KEY_W))
	{
		sound_list_add(sound_man, SFX_JUMP);
	}
}

void time_update(Time* time)
{
    u64 pc = SDL_GetPerformanceCounter();
    u64 freq = SDL_GetPerformanceFrequency();
    time->cur_time = (f64)pc / (f64)freq;

    // Compute frame time
    f64 frametime = time->cur_time - time->last_time;
    if (frametime <= 0.0) frametime = 1.0 / 1000.0; // prevent divide by zero
    time->fps_avg = time->fps_avg * 0.9 + (1.0 / frametime) * 0.1;

	// Don't log every frame
    if (time->frame_counter % 2000 == 0)
        TRACE("FPS: %0.0f", time->fps_avg);

    // Update time for next frames 
    time->dt = frametime;
    time->last_time = time->cur_time;
    time->frame_counter++;
}

// TODO: This needs to be made more robust later
void platform_gamepad_update(AppState* as) {
    SDL_PumpEvents();
    SDL_UpdateGamepads();

    i32 num_gamepads;
    SDL_JoystickID* ids = SDL_GetGamepads(&num_gamepads);

    if (num_gamepads > 0 && as->platform_input.platform_gamepad == NULL) {
        SDL_Gamepad* controller = SDL_OpenGamepad(ids[0]);
        if (controller) {
            INFO("Opened gamepad: %s", SDL_GetGamepadName(controller));
            as->platform_input.platform_gamepad = controller;
        }
    }
}

void frame_begin(AppState* as)
{
    sound_list_clear(&as->sound_manager);
    time_update(&as->time);
}

void frame_end(AppState* as) 
{
    arena_reset(&as->frame_state.frame_arena);
    arena_reset(&as->frame_state.frame_scratch);
}

void game_step(AppState* as)
{
    // game_init(&as->platform_api);
    frame_begin(as);

	platform_gamepad_update(as);
    platform_input(&as->platform_input, &as->game_input);    
    game_input(&as->game_input);

    game_update(as);
    audio_list_build(as, &as->game_input);

    frame_end(as);
}
