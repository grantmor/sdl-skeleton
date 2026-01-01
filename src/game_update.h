#include "platform_sdl.h"

void game_update(AppState* as);

void build_render_list(AppState* as);

void sound_list_add(SoundManager* sound_man, SoundID sound_id);

void clear_sound_list(SoundManager* sound_man);

void build_audio_list(AppState* as, GameInput* game_input);

void update_time(Time* time);

void platform_gamepad_update(AppState* as);

void game_step(AppState* as);

