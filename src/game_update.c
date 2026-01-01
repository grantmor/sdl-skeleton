// #pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

#include "audio.h"
#include "platform_sdl.h"
#include "input.c"

void game_update(AppState* as)
{
	
}

void render_list_build(AppState* as)
{
	
}

void sound_add(SoundManager* sound_man, SoundID sound_id)
{
    // SDL_Log("add_sound() sound_id: %d", sound_id);
    sound_man->playing_sounds[sound_man->num_sounds_to_play++] = sound_id;
    // sound_man->num_sounds_to_play += 1;
}

void sound_list_clear(SoundManager* sound_man)
{
    // for (u32 s=sound_man->num_sounds_to_play; s<=0; s--)
    for (i32 s=sound_man->num_sounds_to_play; s>0; s--)
    {
        sound_man->playing_sounds[s] = SFX_NO_SOUND;
    }
    sound_man->num_sounds_to_play = 0;    
}

void audio_list_build(AppState* as, GameInput* game_input)
{
    SoundManager* sound_man = &as->sound_manager;	
	// Test Audio

	//SDL_Log("bytes queued: %i", SDL_GetAudioStreamQueued(stream));
	if (game_input->keyboard_state.key[KEY_Q] == BUTTON_PRESSED)
	{
		sound_add(sound_man, SFX_COIN);
	}

	if (game_input->keyboard_state.key[KEY_W] == BUTTON_PRESSED)
	{
		sound_add(sound_man, SFX_JUMP);
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
        SDL_Log("FPS: %0.0f", time->fps_avg);

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
            SDL_Log("Opened gamepad: %s", SDL_GetGamepadName(controller));
            as->platform_input.platform_gamepad = controller;
        }
    }
}

void game_step(AppState* as)
{
    sound_list_clear(&as->sound_manager);

    time_update(&as->time);

	platform_gamepad_update(as);
    platform_input(&as->platform_input, &as->game_input);    
    game_input(&as->game_input);

    game_update(as);

    render_list_build(as);
    audio_list_build(as, &as->game_input);
}

