#pragma once

#include "types.h"

#define AXIS_DEADZONE 25
#define NUM_SCANCODES 128
#define NUM_MOUSE_BUTTONS 6

typedef enum 
{
	BUTTON_PRESSED,
	BUTTON_RELEASED,
	BUTTON_IDLE,
	BUTTON_HELD
} ButtonState;

typedef enum {
	NEGATIVE,
	POSITIVE
} Sign;

typedef enum {
	BUTTON_NORTH = 3,
	BUTTON_SOUTH = 0,
	BUTTON_EAST = 1,
	BUTTON_WEST = 2,

	DPAD_UP = 11,
	DPAD_DOWN = 12,
	DPAD_LEFT = 13,
	DPAD_RIGHT = 14,

	SHOULDER_LEFT = 9,
	SHOULDER_RIGHT = 10,

	TRIGGER_LEFT = 15,
	TRIGGER_RIGHT = 16,

	CLICK_LEFT = 7,
	CLICK_RIGHT = 8,

	BUTTON_START = 6,
	BUTTON_SELECT = 4
} ControllerMap;

typedef enum {
	AXIS_LEFTX = 0,
	AXIS_LEFTY = 1,
	AXIS_RIGHTX = 2,
	AXIS_RIGHTY = 3,
	AXIS_TRIGGER_L = 4,
	AXIS_TRIGGER_R = 5
} ControllerAxisMap;

typedef struct {
	ButtonState button[18];
	i16 axis[6];
} ControllerState;

typedef struct {
	ButtonState key[NUM_SCANCODES]; 
} KeyboardState;

typedef struct {
	b8 button[18];
	i16 axis[6];
} PlatformControllerState;

typedef struct {
	b8 key[NUM_SCANCODES];
} PlatformKeyboardState;

typedef enum {
	KEY_A = 4,
	KEY_B = 5,
	KEY_C = 6,
	KEY_D = 7,
	KEY_E = 8,
	KEY_F = 9,
	KEY_G = 10,
	KEY_H = 11,
	KEY_I = 12,
	KEY_J = 13,
	KEY_K = 14,
	KEY_L = 15,
	KEY_M = 16,
	KEY_N = 17,
	KEY_O = 18,
	KEY_P = 19,
	KEY_Q = 20,
	KEY_R = 21,
	KEY_S = 22,
	KEY_T = 23,
	KEY_U = 24,
	KEY_V = 25,
	KEY_W = 26,
	KEY_X = 27,
	KEY_Y = 28,
	KEY_Z = 29,

	KEY_0 = 39,
	KEY_1 = 30,
	KEY_2 = 31,
	KEY_3 = 32,
	KEY_4 = 33,
	KEY_5 = 34,
	KEY_6 = 35,
	KEY_7 = 36,
	KEY_8 = 37,
	KEY_9 = 38,

	KEY_ESCAPE = 41,
	KEY_TAB = 43,
	KEY_SPACE = 44,
	KEY_MINUS = 45,
	KEY_EQUALS = 46,
	KEY_LEFTBRACKET = 47,
	KEY_RIGHTBRACKET = 48,
	KEY_BACKSLASH = 49,
	KEY_SEMICOLON = 51,
	KEY_APOSTROPHE = 52,
	KEY_GRAVE = 53,
	KEY_COMMA = 54,
	KEY_PERIOD = 55,
	KEY_SLASH = 56,

	KEY_F1 = 58,
	KEY_F2 = 59,
	KEY_F3 = 60,
	KEY_F4 = 61,
	KEY_F5 = 62,
	KEY_F6 = 63,
	KEY_F7 = 64,
	KEY_F8 = 65,
	KEY_F9 = 66,
	KEY_F10 = 67,
	KEY_F11 = 68,
	KEY_F12 = 69
} KeyboardMap;

typedef enum {
	MOUSE_BUTTON_LEFT = 0, 
	MOUSE_BUTTON_MIDDLE = 1,
	MOUSE_BUTTON_RIGHT = 2,
	MOUSE_BUTTON_SIDE_1 = 3,
	MOUSE_BUTTON_SIDE_2 = 4
} MouseButtonMap;

typedef struct {
	f32 pos_x;
	f32 rel_x;
	f32 pos_y;
	f32 rel_y;
	b8 button[8];
} PlatformMouseState;

typedef struct {
	f32 pos_x;
	f32 rel_x;
	f32 pos_y;
	f32 rel_y;
	ButtonState button[8];
} MouseState;
