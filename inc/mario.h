

#ifndef __MARIO_H
#define __MARIO_H

#include "internal/charmap.h"
#include "internal/common_types.h"
#include "internal/common_nes.h"
#include "internal/memory.h"
#include "internal/metasprites.h"
#include "internal/options.h"

#include <peekpoke.h>


// Gameplay callbacks

// Called at the end of the Area init routine
void init_area_callback();

// Called at the end of every game frame. If the game is paused, this is NOT called.
void after_game_callback();

// Called at the end of every frame, after the game frame ends.
// This is called always, even when paused.
void after_frame_callback();

// Title screen callbacks
#if USE_CUSTOM_TITLESCREEN

// Called every frame until the OperMode_Task increases by 1
// So you can spread out the setup work over as many frames as needed
void title_screen_setup();

// Called every frame in the title screen
void title_screen_menu();
#endif

// Somewhat useful functions

extern u8 VRAM_Buffer_Offset[2];
extern u8 VRAM_AddrTable_Low[];
extern u8 VRAM_AddrTable_High[];
void flush_vram_buffer();

#endif
