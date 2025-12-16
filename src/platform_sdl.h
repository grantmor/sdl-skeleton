#pragma once

#include <SDL3/SDL.h>
#include "types.h"

typedef enum {
	NEGATIVE,
	POSITIVE
} Sign;

/*
	FIXME: ControllerState and KeyboardState
	should probably be a game header, not a platform header
*/

typedef struct {
	i16 axis_lx;
	i16 axis_ly;
	i16 axis_rx;
	i16 axis_ry; 

	i16 axis_lt;
	i16 axis_rt; 

	u8 button_north;
	u8 button_south;
	u8 button_east;
	u8 button_west;
	
	u8 dpad_up;
	u8 dpad_down;
	u8 dpad_left;
	u8 dpad_right;

	u8 shoulder_left;	
	u8 shoulder_right;

	u8 trigger_left;
	u8 trigger_right;

	u8 click_left;
	u8 click_right;

	u8 button_start;
	u8 button_select;
} ControllerState;

typedef struct {
	u8 KEY_0;
	u8 KEY_1;
	u8 KEY_2;
	u8 KEY_3;
	u8 KEY_4;
	u8 KEY_5;
	u8 KEY_6;
	u8 KEY_7;
	u8 KEY_8;
	u8 KEY_9;
	u8 KEY_A;
	u8 KEY_B;
	u8 KEY_C;
	u8 KEY_D;
	u8 KEY_E;
	u8 KEY_F;
	u8 KEY_G;
	u8 KEY_H;
	u8 KEY_I;
	u8 KEY_J;
	u8 KEY_K;
	u8 KEY_L;
	u8 KEY_M;
	u8 KEY_N;
	u8 KEY_O;
	u8 KEY_P;
	u8 KEY_Q;
	u8 KEY_R;
	u8 KEY_S;
	u8 KEY_T;
	u8 KEY_U;
	u8 KEY_V;
	u8 KEY_W;
	u8 KEY_X;
	u8 KEY_Y;
	u8 KEY_Z;

	u8 KEY_MINUS;          
	u8 KEY_EQUALS;         
	u8 KEY_LEFTBRACKET;    
	u8 KEY_RIGHTBRACKET;   
	u8 KEY_BACKSLASH;      
	u8 KEY_SEMICOLON;      
	u8 KEY_APOSTROPHE;     
	u8 KEY_GRAVE;          
	u8 KEY_COMMA;          
	u8 KEY_PERIOD;         
	u8 KEY_SLASH;          

	u8 KEY_SPACE;
	u8 KEY_TAB;
} KeyboardState;

typedef struct {
	f32 pos_x;
	f32 rel_x;
	f32 pos_y;
	f32 rel_y;
	u8 click_left;
	u8 click_right;
	u8 click_middle;
	u8 side_1;
	u8 side_2;
	u8 wheel_up;
	u8 wheel_down;
} MouseState;

typedef struct {
	f64 last_time;
	f64 cur_time;
	f64 dt;
	f64 fps_avg;
	u64 frame_counter;
} Time;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;

	Time* time;

	//Input
	SDL_Gamepad* sdl_gamepad;
	ControllerState* controller_state;
	KeyboardState* keyboard_state;
	MouseState* mouse_state;
} AppState;
