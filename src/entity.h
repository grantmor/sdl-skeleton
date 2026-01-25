#pragma once
#include "types.h"

#define MAX_ANIMATIONS 32
#define MAX_ANIMATION_FRAMES 128
#define MAX_TILES 32
#define MAX_ENTITIES 1024

typedef enum {
   MAIN_IDLE, 
   MAIN_WALK,
   MAIN_RUN,
   SPRITE_ANIMATION_COUNT,
} SpriteAnimationKind;

typedef enum {
    IDLE,
    WALK,
    RUN,
    SPRITE_ANIMATION_CLASS_COUNT,
} SpriteAnimationClass;

typedef enum {
    MAIN,
    SPRITE_KIND_COUNT,
} SpriteKind;

typedef struct {
    u32 start_frame;
    u16 frame_count;
    f32 anim_speed;
    b8 loop;    
} SpriteAnimation;

typedef struct {
    f32 time_elapsed;
    u16 frame;
} SpriteAnimationState;

typedef struct {
    SpriteKind sprite_kind;
    SpriteAnimationKind current_animation;
    SpriteAnimationState anim_state;
} Sprite;

typedef struct {
    Pos2 pos;
    Dir2 dir;
    Sprite sprite;
} Entity;

typedef struct {
	u32 index;
	u32 generation;
} EntityHandle;

typedef struct {	
	Entity* entities;
	u32 entity_count;
} EntityStore;
