#include "platform_sdl.h"

void game_update(AppState* as);

void build_render_list(AppState* as);

void add_sound(SoundManager* sound_man, SoundID sound_id);

void clear_sound_list(SoundManager* sound_man);

void build_audio_list(AppState* as, GameInput* game_input);

void update_time(Time* time);

void platform_update_gamepads(AppState* as);

void game_step(AppState* as);

